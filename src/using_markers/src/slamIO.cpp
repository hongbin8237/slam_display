/*
 * =====================================================================================
 *
 *       Filename:  slamIO.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/01/2014 11:10:35 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Hong-Bin Yoon (), yoon48@illinois.edu
 *   Organization:  
 *
 * =====================================================================================
 */
#include "slamIO.h"

    void
SlamIO::get_slamprojections( )
{

    slammatches.clear();
    slamprojection pj;

    /* get X and V */
    char str[32];
    strcpy( str, (isHex) ? "%lx,%lx,%lx,%lx,%lx,%lx" : "%lf,%lf,%lf,%lf,%lf,%lf" );
    get_val(fp, "XV", str, &X[0], &X[1], &X[2], &V[0], &V[1], &V[2]  );

    /* get Feature Points */
    strcpy( str, (isHex) ? "%d,%lx,%lx,%lx" : "%d,%lf,%lf,%lf" );
    while( get_val( fp, "slam", str, &pj.id,&pj.source.x, &pj.source.y, 
                &pj.source.z )!=-1 )
    {
        slammatches.push_back(pj);
    }
    return ;

}
void
SlamIO::update()
{
    get_slamprojections();
    return;
}

    FILE *
SlamIO::open_source ( const char *fn )
{
    FILE *fp;
    if( (fp=fopen(fn,"r"))==NULL )
        err_sys("fopen body");
    return fp ;
} 

    int
SlamIO::get_val ( FILE* fp, const char *str, const char *fmt, ... )
{
    int rv;
    char line[MAXLINE];
    va_list ap;
    va_start(ap,fmt);

    if( (fgets(line,MAXLINE,fp ))==NULL )
        err_sys("fgets %s", str);
    rv=vsscanf( line, fmt, ap );
    va_end(ap);

    return rv ;
}
