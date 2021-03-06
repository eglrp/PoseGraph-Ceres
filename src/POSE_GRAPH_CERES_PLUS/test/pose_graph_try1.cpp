#include "common_include.h"
#include "config.h"
#include "Frame.h"
#include "MapPoint.h"
#include "converter.h"
#include "SequenceRun.h"
#include "ORBmatcher.h"

/*g2o parts*/
#include <g2o/types/slam3d/types_slam3d.h>
#include <g2o/core/sparse_optimizer.h>
#include <g2o/core/block_solver.h>
#include <g2o/core/factory.h>
#include <g2o/core/optimization_algorithm_factory.h>
#include <g2o/core/optimization_algorithm_gauss_newton.h>
#include <g2o/solvers/eigen/linear_solver_eigen.h>
#include <g2o/core/robust_kernel.h>
#include <g2o/core/robust_kernel_impl.h>
#include <g2o/core/optimization_algorithm_levenberg.h>
#include <g2o/core/estimate_propagator.h>

#include <fstream>

using namespace std;
using namespace POSE_GRAPH;

struct RESULT_OF_PNP
{
    cv::Mat rvec;
    cv::Mat tvec;
    int inliers;
};
double normofTransform( cv::Mat rvec, cv::Mat tvec );


void checkForPoseGraph(vector<Frame>& vFrames, Frame &currentFrame, g2o::SparseOptimizer &optimizer);
void checkFrame(Frame &frame1, Frame &frame2, g2o::SparseOptimizer &optimizer);
std::vector<Frame> getCandidateFrames(vector<Frame>& vFrames, Frame &currentFrame);
int findFeatureMatches(Frame &lastFrame, Frame &currentFrame, vector<DMatch> &matches, string method = "BF");
RESULT_OF_PNP motionEstimate(Frame &frame1, Frame &frame2);

int main(int argc, char *argv[])
{



    Config::setParameterFile("/home/m/ws_orb2/src/POSE_GRAPH/config/config08.yaml");
    string dir = Config::get<string>("sequence_dir");
    /*get the sequence length*/
    int sequenceLength = Config::get<int>("sequence_length");

    /*g2o parts*/

    g2o::SparseOptimizer optimizer;
    g2o::LinearSolverEigen<g2o::BlockSolver_6_3::PoseMatrixType> * linearSolver;

    linearSolver = new g2o::LinearSolverEigen<g2o::BlockSolver_6_3::PoseMatrixType>();
    linearSolver->setBlockOrdering(false);

    g2o::BlockSolver_6_3 * solver_ptr = new g2o::BlockSolver_6_3(linearSolver);

    g2o::OptimizationAlgorithmLevenberg* solver = new g2o::OptimizationAlgorithmLevenberg(solver_ptr);
    optimizer.setAlgorithm(solver);
    optimizer.setVerbose(true);

    SequenceRun* sequenceRun = new SequenceRun();
    vector<Frame> vFrames;

    ofstream outFile;
    outFile.open("../camera_poses.txt");

    for(int32_t i = 0; i < sequenceLength; i++)
    {
        char base_name[256];
        sprintf(base_name,"%06d.png",i);
        string left_img_file_name = dir + "/image_0/" + base_name;
        string right_img_file_name = dir + "/image_1/" + base_name;
        Mat Left = cv::imread(left_img_file_name,CV_LOAD_IMAGE_GRAYSCALE);
        Mat Right = cv::imread(right_img_file_name,CV_LOAD_IMAGE_GRAYSCALE);
        cout << RESET"Process :" << i << endl;

        sequenceRun->GrabImageStereo(Left,Right,0.0);
        Frame currentFrame = sequenceRun->mCurrentFrame;

        /*add the frame vertex*/
        g2o::VertexSE3 * vSE3 = new g2o::VertexSE3();
        vSE3->setEstimate(Eigen::Isometry3d::Identity());
        vSE3->setId(currentFrame.mnId);
        optimizer.addVertex(vSE3);
        if(currentFrame.mnId == 0)
            vSE3->setFixed(true);
        else
            vSE3->setFixed(false);


        /*add vertex and edges to generate pose graph*/
        if(i > 0)
        {
            vector<Frame> vCandidateFrames = getCandidateFrames(vFrames,currentFrame);

            checkForPoseGraph(vCandidateFrames, currentFrame, optimizer);
        }

        //cv::imshow("currentFrame", sequenceRun->mImGray);


        //cv::waitKey(27);

        vFrames.push_back(currentFrame);
        Mat pose = currentFrame.mTwc;

        //cout << "currentframe.mTwc = " << pose << endl;
        for(int i = 0; i < 3; i++)
        {
            for(int j = 0; j < 4; j++)
            {
                if(i==2 && j==3)
                    outFile << pose.at<float>(i,j);
                else
                    outFile << pose.at<float>(i,j) << " ";

            }
        }
        outFile << endl;


    }

    cout<<RESET"optimizing pose graph, vertices: "<<optimizer.vertices().size()<<endl;
    optimizer.save("../result/g2o/result_before.g2o");


    optimizer.initializeOptimization();


    g2o::EstimatePropagatorCost costFunction(&optimizer);
    optimizer.computeInitialGuess(costFunction);


    optimizer.optimize( 1000 );
    optimizer.save( "../result/g2o/result_after.g2o" );
    cout<<"Optimization done."<<endl;






    return 0;
}

void checkForPoseGraph(vector<Frame> &vFrames, Frame &currentFrame, g2o::SparseOptimizer &optimizer)
{
    for(size_t i = 0; i < vFrames.size(); i++)
    {
        checkFrame(vFrames[i], currentFrame, optimizer);
    }


}

void checkFrame(Frame &frame1, Frame &frame2, g2o::SparseOptimizer &optimizer)
{
    /*first we need to check this two frames --> use ORBmatcher*/

    ORBmatcher matcher(0.7,true);

    int nmatches = matcher.MatcheTwoFrames(frame2,frame1,5,false);
    //vector<DMatch> matches;
    //int nmatches = findFeatureMatches(frame1,frame2,matches);
    cout << "matches = " << nmatches << " ";

    if(frame2.mnId - frame1.mnId == 1)
    {
        // EDGE
        g2o::EdgeSE3* edge = new g2o::EdgeSE3();


        edge->setVertex( 0, optimizer.vertex(frame1.mnId ));
        edge->setVertex( 1, optimizer.vertex(frame2.mnId ));
        edge->setRobustKernel( new g2o::RobustKernelHuber() );

        // imformation Matrix
        Eigen::Matrix<double, 6, 6> information = Eigen::Matrix< double, 6,6 >::Identity();
        //        information(0,0) = information(1,1) = information(2,2) = 100;
        //        information(3,3) = information(4,4) = information(5,5) = 100;

        edge->setInformation( information );

        Eigen::Isometry3d T = Converter::toIsometry3d(frame2.mTcw * frame1.mTwc);
        edge->setMeasurement( T );
        optimizer.addEdge(edge);
    }

    else if(nmatches > 180)
    {

        /*now we check the motion and inliers of this two frame*/
        RESULT_OF_PNP result = motionEstimate(frame1,frame2);
        double norm = normofTransform(result.rvec,result.tvec);

        if(result.inliers > 80 && norm < 1.2)
        {
            // EDGE
            g2o::EdgeSE3* edge = new g2o::EdgeSE3();


            edge->setVertex( 0, optimizer.vertex(frame1.mnId ));
            edge->setVertex( 1, optimizer.vertex(frame2.mnId ));
            edge->setRobustKernel( new g2o::RobustKernelHuber() );

            // imformation Matrix
            Eigen::Matrix<double, 6, 6> information = Eigen::Matrix< double, 6,6 >::Identity();
            edge->setInformation( information );
            Eigen::Isometry3d T = Converter::toIsometry3d(frame2.mTcw * frame1.mTwc);
            edge->setMeasurement( T );
            optimizer.addEdge(edge);

            cout << BOLDYELLOW"add a new edge! " << frame2.mnId << " --> " << frame1.mnId <<  endl;
        }

    }
    cout << endl;

}

std::vector<Frame> getCandidateFrames(vector<Frame>& vFrames, Frame &currentFrame)
{
    /*here we need to get the candidate frames according to the Twc*/
    std::vector<Frame> candidates;
    //cout << BOLDGREEN"candidate frames id = ";

    for(vector<Frame>::const_iterator vit = vFrames.begin(); vit!=vFrames.end();vit++)
    {
        if(currentFrame.mnId - Frame(*vit).mnId > 5 && currentFrame.mnId - Frame(*vit).mnId < 100)
            continue;

        cv::Mat twc = Frame(*vit).GetCameraCenter();

        if(currentFrame.mnId - Frame(*vit).mnId < 5) /*near by frames --> can write to the config files*/
        {
            candidates.push_back(*vit);
        }
        bool isInRange = currentFrame.isInSearchRange(twc);
        if(isInRange)
        {
            if(currentFrame.mnId - Frame(*vit).mnId >= 100 )
            {
                candidates.push_back(*vit);
                cout << Frame(*vit).mnId << " ";
                cout << BOLDRED"may be we got a loop!" << endl;
            }

        }

    }

    cout << endl;

    return candidates;

}

int findFeatureMatches(Frame &lastFrame,
                       Frame &currentFrame,
                       vector<DMatch> &matches,
                       string method)
{
    /*
      * define the detect param: use ORB
      */
    Ptr<ORB> orb = ORB::create(1000);
    Mat descriptors;
    std::vector<KeyPoint> keypoints1,keypoints2;
    /*
      * use detect() function to detect keypoints
      */
    orb-> detect(currentFrame.mImageGray, keypoints1);
    /*
      * conpute the extractor and show the keypoints
      */
    orb-> compute(currentFrame.mImageGray, keypoints1, descriptors);

    Mat testDescriptors;
    orb->detect(lastFrame.mImageGray, keypoints2);
    orb->compute(lastFrame.mImageGray, keypoints2,testDescriptors);

    //vector<DMatch> match;
    /*
      * FLANN
      */

    if(method == "KNN" || method == "knn")
    {
        testDescriptors.convertTo(testDescriptors,CV_32S);
        descriptors.convertTo(testDescriptors,CV_32S);
        flann::Index flannIndex(testDescriptors, flann::LshIndexParams(12, 20, 2), cvflann::FLANN_DIST_HAMMING);



        /*Match the feature*/
        Mat matchIndex(descriptors.rows, 2, CV_32S);
        Mat matchDistance(descriptors.rows, 2, CV_32S);
        flannIndex.knnSearch(descriptors.rows, matchIndex, matchDistance, 2, flann::SearchParams());

        //vector<DMatch> goodMatches;
        for (int i = 0; i < matchDistance.rows; i++)
        {
            if(matchDistance.at<float>(i,0) < 0.75 * matchDistance.at<float>(i, 1))
            {
                DMatch dmatchs(i, matchIndex.at<int>(i,0), matchDistance.at<float>(i,1));
                matches.push_back(dmatchs);
            }
        }

    }
    else if(method == "BF")
    {
        vector<DMatch> tmpMatches;
        BFMatcher matcher(NORM_HAMMING);
        matcher.match(descriptors, testDescriptors, tmpMatches);
        double min_dist = 10000, max_dist = 0;
        for(int i = 0; i < descriptors.rows; i++)
        {
            double dist = tmpMatches[i].distance;
            if(dist < min_dist) min_dist = dist;
            if(dist > max_dist) max_dist = dist;
        }
        cout << "--Max dist = " << max_dist << endl;
        cout << "--Min dist = " << min_dist << endl;

        for(int i =0; i < descriptors.rows; i++)
        {
            if(tmpMatches[i].distance <= max(2*min_dist, 30.0))
            {
                matches.push_back(tmpMatches[i]);
            }
        }

    }

    Mat resultImage;
    drawMatches(currentFrame.mImageGray, keypoints1, lastFrame.mImageGray, keypoints2, matches, resultImage);
    imshow("result of Image", resultImage);
    cv::waitKey(27);

    cout << "We got " << matches.size() << " good Matchs" << endl;
    return (int)matches.size();
}

/*we need a simple function to estimate the inliers and pose*/
RESULT_OF_PNP motionEstimate(Frame &frame1, Frame &frame2)
{
    double fx_ = Config::get<double>("Camera.fx");
    double fy_ = Config::get<double>("Camera.fy");
    double cx_ = Config::get<double>("Camera.cx");
    double cy_ = Config::get<double>("Camera.cy");

    RESULT_OF_PNP result;
    map<int,int> matches = frame2.matchesId;
    vector<cv::Point3f> pts_obj;
    vector< cv::Point2f > pts_img;


    for(map<int,int>::const_iterator mit = matches.begin(); mit!=matches.end(); mit++)
    {

        cv::Point2f p = frame1.mvKeys[mit->second].pt;
        pts_img.push_back( cv::Point2f( frame2.mvKeys[mit->first].pt ) );

        // 将(u,v,d)转成(x,y,z)
        cv::Point2f pt ( p.x, p.y );
        float depth = frame1.mvDepth[mit->second];

        //cv::Point3f pd = pCamera->pixel2camera(pt,depth);
        cv::Mat p3d = frame2.pixel2Camera(pt.x,pt.y,depth);
        cv::Point3f pd(p3d.at<float>(0),p3d.at<float>(1),p3d.at<float>(2));
        pts_obj.push_back( pd );
    }


    if (pts_obj.size() ==0 || pts_img.size()==0)
    {
        result.inliers = -1;
        return result;
    }


    double camera_matrix_data[3][3] = {
        {fx_, 0, cx_},
        {0, fy_, cy_},
        {0, 0, 1}
    };

    // 构建相机矩阵
    cv::Mat cameraMatrix( 3, 3, CV_64F, camera_matrix_data );
    cv::Mat rvec, tvec, inliers;
    // 求解pnp
    cv::solvePnPRansac( pts_obj, pts_img, cameraMatrix, cv::Mat(), rvec, tvec, false, 200, 2.0, 0.99, inliers );

    result.rvec = rvec;
    result.tvec = tvec;
    result.inliers = inliers.rows;
    //    cout << "inliers = " <<  endl << inliers << endl;

    /*remove the outliers*/
    map<int,int> matches_out;
    for(int i = 0; i < inliers.rows; i++)
    {
        int row = inliers.at<int>(i);
        map<int,int>::const_iterator map_iter_start = matches.begin();

        if (row == 0)
        {
            matches_out.insert(make_pair(map_iter_start->first,map_iter_start->second));
        }
        else
        {
            while(row)
            {
                map_iter_start++;
                row--;
            }
            matches_out.insert(make_pair(map_iter_start->first,map_iter_start->second));
        }


    }

    //cout << "matches out.size = " << matches_out.size() << endl;
    frame2.matchesId.clear();
    frame2.matchesId = matches_out;

    return result;
}

double normofTransform( cv::Mat rvec, cv::Mat tvec )
{
    return fabs(min(cv::norm(rvec), 2*M_PI-cv::norm(rvec)))+ fabs(cv::norm(tvec));
}


