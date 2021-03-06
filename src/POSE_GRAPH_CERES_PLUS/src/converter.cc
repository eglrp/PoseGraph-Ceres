/**
* This file is part of ORB-SLAM2.
*
* Copyright (C) 2014-2016 Raúl Mur-Artal <raulmur at unizar dot es> (University of Zaragoza)
* For more information see <https://github.com/raulmur/ORB_SLAM2>
*
* ORB-SLAM2 is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* ORB-SLAM2 is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with ORB-SLAM2. If not, see <http://www.gnu.org/licenses/>.
*/


#include "converter.h"
#include "common_include.h"

namespace POSE_GRAPH
{
std::vector<cv::Mat> Converter::toDescriptorVector(const cv::Mat &Descriptors)
{
    std::vector<cv::Mat> vDesc;
    vDesc.reserve(Descriptors.rows);
    for (int j=0;j<Descriptors.rows;j++)
        vDesc.push_back(Descriptors.row(j));

    return vDesc;
}

cv::Mat Converter::toCvMat(const Eigen::Matrix<double,4,4> &m)
{
    cv::Mat cvMat(4,4,CV_32F);
    for(int i=0;i<4;i++)
        for(int j=0; j<4; j++)
            cvMat.at<float>(i,j)=m(i,j);

    return cvMat.clone();
}

cv::Mat Converter::toCvMat(const Eigen::Matrix3d &m)
{
    cv::Mat cvMat(3,3,CV_32F);
    for(int i=0;i<3;i++)
        for(int j=0; j<3; j++)
            cvMat.at<float>(i,j)=m(i,j);

    return cvMat.clone();
}

cv::Mat Converter::toCvMat(const Eigen::Matrix<double,3,1> &m)
{
    cv::Mat cvMat(3,1,CV_32F);
    for(int i=0;i<3;i++)
        cvMat.at<float>(i)=m(i);

    return cvMat.clone();
}

cv::Mat Converter::toCvMat(const Eigen::Quaterniond &q)
{
    Eigen::Matrix3d eigMat =  q.toRotationMatrix();
    cv::Mat R = toCvMat(eigMat);
    return R.clone();

}

cv::Mat Converter::toCvMat(const Pose3d &pose)
{
    cv::Mat R = toCvMat(pose.q.toRotationMatrix());
    cv::Mat t = toCvMat(pose.p);
    cv::Mat T = cv::Mat::eye(4,4,CV_32F);
    R.copyTo(T.rowRange(0,3).colRange(0,3));
    t.copyTo(T.rowRange(0,3).col(3));
    return T.clone();
}

cv::Mat Converter::toCvSE3(const Eigen::Matrix<double,3,3> &R, const Eigen::Matrix<double,3,1> &t)
{
    cv::Mat cvMat = cv::Mat::eye(4,4,CV_32F);
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            cvMat.at<float>(i,j)=R(i,j);
        }
    }
    for(int i=0;i<3;i++)
    {
        cvMat.at<float>(i,3)=t(i);
    }

    return cvMat.clone();
}

Eigen::Matrix<double,3,1> Converter::toVector3d(const cv::Mat &cvVector)
{
    Eigen::Matrix<double,3,1> v;
    cv::Mat cvVectorCopy;
    if(cvVectorCopy.type() == 6)
    {
       cvVector.convertTo(cvVectorCopy,CV_32FC1);
       v << cvVectorCopy.at<float>(0), cvVectorCopy.at<float>(1), cvVectorCopy.at<float>(2);
    }
    else
        v << cvVector.at<float>(0), cvVector.at<float>(1), cvVector.at<float>(2);

    return v;
}

Eigen::Matrix<double,3,1> Converter::toVector3d(const cv::Point3f &cvPoint)
{
    Eigen::Matrix<double,3,1> v;
    v << cvPoint.x, cvPoint.y, cvPoint.z;

    return v;
}

Eigen::Matrix<double,3,3> Converter::toMatrix3d(const cv::Mat &cvMat3)
{
    Eigen::Matrix<double,3,3> M = Eigen::Matrix3d::Identity();
    cv::Mat cvMat3Copy;
    if(cvMat3.type() == 6)
    {
        cvMat3.convertTo(cvMat3Copy,CV_32FC1);
        M << cvMat3Copy.at<float>(0,0), cvMat3Copy.at<float>(0,1), cvMat3Copy.at<float>(0,2),
             cvMat3Copy.at<float>(1,0), cvMat3Copy.at<float>(1,1), cvMat3Copy.at<float>(1,2),
             cvMat3Copy.at<float>(2,0), cvMat3Copy.at<float>(2,1), cvMat3Copy.at<float>(2,2);
    }
    else
    {

        M << cvMat3.at<float>(0,0), cvMat3.at<float>(0,1), cvMat3.at<float>(0,2),
             cvMat3.at<float>(1,0), cvMat3.at<float>(1,1), cvMat3.at<float>(1,2),
             cvMat3.at<float>(2,0), cvMat3.at<float>(2,1), cvMat3.at<float>(2,2);
    }




    return M;
}

Eigen::Quaterniond Converter::toQuaternion(const cv::Mat &M)
{
    Eigen::Matrix<double,3,3> eigMat = toMatrix3d(M);
    Eigen::Quaterniond q(eigMat);
    return q;
}


uint8_t* Converter::toPng(cv::Mat image)
{
    uint8_t* img_data = new uint8_t[image.rows*image.cols];
    int k = 0;
    for(int v = 0; v < image.rows; v++)
    {
        for(int u = 0; u < image.cols; u++)
        {
            img_data[k] = (uint8_t)image.at<uchar>(v,u);
            k++;
        }
    }

    return img_data;
}

cv::Mat Converter::toCvMat(const Eigen::Vector3d &m, int flag = 1)
{
    if(flag)
    {
        cv::Mat T = cv::Mat::zeros(3,1,CV_32FC1);
        for(int i = 0; i < 3; i++)
        {
            T.at<float>(i,0) = m(i,0);
        }
        return T.clone();
    }
    else
    {
        cv::Mat T = cv::Mat::zeros(3,1,CV_32FC1);
        for(int i = 0; i < 3; i++)
        {
            T.at<float>(i,0) = m(i,0);
        }
        return T.clone();
    }

}

Eigen::Isometry3d Converter::toIsometry3d(const cv::Mat &M)
{
    Eigen::Matrix3d r;
    for(int i =0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            r(i,j) = M.at<float>(i,j);
        }
    }

    Eigen::Isometry3d T = Eigen::Isometry3d::Identity();

    Eigen::AngleAxisd angle(r);
    T = angle;
    T(0,3) = (double)M.at<float>(0,3);
    T(1,3) = (double)M.at<float>(1,3);
    T(2,3) = (double)M.at<float>(2,3);
    return T;


}


Pose3d Converter::toPose3d(const cv::Mat &Twc)
{
    Pose3d pose;
    cv::Mat Rwc = Twc.rowRange(0,3).colRange(0,3);
    cv::Mat twc = Twc.rowRange(0,3).col(3);

    Eigen::Vector3d p = toVector3d(twc);
    Eigen::Quaterniond q = toQuaternion(Rwc);

    pose.p = p;
    pose.q = q;

    return pose;
}



}


