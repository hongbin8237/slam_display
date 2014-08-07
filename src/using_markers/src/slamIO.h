/*
 * =====================================================================================
 *
 *       Filename:  slamIO.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/01/2014 10:56:23 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Hong-Bin Yoon (), yoon48@illinois.edu
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef SLAMIO_H
#define SLAMIO_H
#include <cv.h>
#include "ourerr.hpp"

#define MAXLINE 1024

struct slamprojection {
    slamprojection(){};
    slamprojection( const cv::Point3d& s, int i ) : source(s), id(i) {};
    cv::Point3d source;
    int id;
};


typedef struct slamprojection Slamprojection;

class SlamIO
{
    public:

	void jotmang(){return;}
        /* ====================  LIFECYCLE     ======================================= */
        SlamIO( const char *fn, bool ih) : isHex(ih){
            set_file(fn);
        }
        /* ====================  MUTATORS     ======================================= */
        void set_file( const char *fn){
            fp=open_source(fn);
        }
        void update();
        /* ====================  DATA MEMBERS  ======================================= */
        std::vector<slamprojection> slammatches;
        FILE *fp;
        bool isHex;
        cv::Vec3d X,V;

        /* ====================  METHODS       ======================================= */
        FILE* open_source ( const char *fn );
        int get_val ( FILE* fp, const char *str, const char *fmt, ... );
        void get_slamprojections();
};


#endif
