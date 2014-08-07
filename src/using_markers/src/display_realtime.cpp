/*
 * =====================================================================================
 *
 *       Filename:  display_realtime.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/10/2014 01:11:59 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Hong-Bin Yoon (), yoon48@illinois.edu
 *   Organization:  
 *
 * =====================================================================================
 */



#include <stdlib.h>
#include "slamIO.h"
#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <iostream>
#include <fstream>
#include <cmath>
//typedef std::map<int,geometry_msgs::Point*> ptMap;
typedef std::map<int,int> ptMap;
typedef ptMap::iterator ptMapIter;
typedef std::vector<geometry_msgs::Point>::iterator ptIter;
typedef std::vector<slamprojection>::iterator matchIter;
std::map<int, geometry_msgs::Point> pts;

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
    int
main ( int argc, char *argv[] )
{
    if( argc>2 )
    {
        printf("Usage: %s\n\
        Reads in SLAM output from fifo:\n\
        x,y,z,vx,vy,vz\n features ... \n", argv[0] );
        exit(EXIT_FAILURE);
    }

    //SlamSensor slamsense( stdin, false);
    //SlamIO slamsense("/home/hong-bin/catkin_ws/src/using_markers/data/slamout.txt", false);
    SlamIO slamsense(argv[1], false);
    ros::init(argc, argv, "display");
    ros::NodeHandle n;
    ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 10);
    visualization_msgs::Marker points, line_strip;
    points.header.frame_id = line_strip.header.frame_id = "/my_frame";
    points.header.stamp = line_strip.header.stamp = ros::Time::now();
    points.ns = line_strip.ns = "display";
    points.action = line_strip.action =  visualization_msgs::Marker::ADD;
    points.pose.orientation.w = line_strip.pose.orientation.w = 1.0;
    ptMap mymap;
    points.id = 0;
    line_strip.id = 1;

    points.type = visualization_msgs::Marker::SPHERE_LIST;
    line_strip.type = visualization_msgs::Marker::LINE_STRIP;

    // POINTS markers use x and y scale for width/height respectively
    points.scale.x = 1;
    points.scale.y = 1;
    points.scale.z = 1;

    // LINE_STRIP/LINE_LIST markers use only the x component of scale, for the line width
    line_strip.scale.x = 0.5;

    // Points = features = blue
    points.color.b = 1.0f;
    points.color.a = 1.0;

    // Line strip = X = red
    line_strip.color.r = 1.0;
    line_strip.color.a = 1.0;
    ros::Rate r(30);

    float f = 0.0;


    while (ros::ok())
    {

        /* Read SLAM output */
        slamsense.update();
        //slamsense.get_slamprojections();


        /* Update Points */
        matchIter match = slamsense.slammatches.begin();

        for( ; match != slamsense.slammatches.end(); ++match)
        {
            geometry_msgs::Point pt;
            pt.x = match->source.x;
            pt.y = match->source.y;
            pt.z = match->source.z; 

            int existingPoint = mymap.count(match->id);
            if(existingPoint)
            {
                ptMapIter it;
                it = mymap.find(match->id);
                points.points[it->second] = pt;                
            }
            else   // insert new point
            {
                int pSize = points.points.size();
                points.points.push_back(pt);
                mymap.insert(std::pair<int,int>(match->id, pSize) );
            }    

        }

        /* plot body trajectory */
        geometry_msgs::Point p;
	    p.x = slamsense.X[0];
    	p.y = slamsense.X[1];
        p.z = slamsense.X[2];
        line_strip.points.push_back(p);

        /* publish data */
        marker_pub.publish(points);
        marker_pub.publish(line_strip);

        ros::spinOnce();
	    r.sleep();
    }
        
        

    return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */

