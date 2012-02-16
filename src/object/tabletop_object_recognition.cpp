/*********************************************************************
 *
 *  Copyright (c) 2009, Willow Garage, Inc.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of the Willow Garage nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************/

// Author(s): Marius Muja and Matei Ciocarlie
#include <string>

#include <ros/ros.h>

#include <sensor_msgs/PointCloud.h>

#include <visualization_msgs/Marker.h>

#include <tf/transform_listener.h>

#include <household_objects_database_msgs/DatabaseModelPose.h>
#include <household_objects_database_msgs/DatabaseModelPoseList.h>
#include <household_objects_database_msgs/GetModelMesh.h>

#include "tabletop_object_detector/exhaustive_fit_detector.h"
#include "tabletop_object_detector/marker_generator.h"
#include "tabletop_object_detector/iterative_distance_fitter.h"
#include "tabletop/Table.h"

#include <tabletop/object/tabletop_object_detector.h>

namespace tabletop_object_detector
{
  TabletopObjectRecognizer::TabletopObjectRecognizer()
  {
    num_markers_published_ = 1;
    current_marker_id_ = 1;

    std::string get_model_mesh_srv_name;
    /*priv_nh_.param < std::string > ("get_model_mesh_srv", get_model_mesh_srv_name, "get_model_mesh_srv");
    while (!ros::service::waitForService(get_model_mesh_srv_name, ros::Duration(2.0)) && nh_.ok())
    {
      ROS_INFO("Waiting for %s service to come up", get_model_mesh_srv_name.c_str());
    }
    if (!nh_.ok())
      exit(0);
    get_model_mesh_srv_ = nh_.serviceClient < household_objects_database_msgs::GetModelMesh
                          > (get_model_mesh_srv_name, true);
    //ask fitter to load models from database
    priv_nh_.param < std::string > ("model_set", model_set_, "");*/
    detector_.loadDatabaseModels(model_set_);

    //initialize operational flags
    fit_merge_threshold_ = 0.05;
    min_marker_quality_ = 0.003;
  }

  double
  TabletopObjectRecognizer::fitDistance(const ModelFitInfo &m1, const ModelFitInfo &m2)
  {
    double dx = m1.getPose().position.x - m2.getPose().position.x;
    double dy = m1.getPose().position.y - m2.getPose().position.y;
    double d = dx * dx + dy * dy;
    return sqrt(d);
  }
} //namespace tabletop_object_detector
