#ifndef CHEBYSHEV_H
#define CHEBYSHEV_H

//==================================================================
/**
 *  chebyshev.h -- C++ functions to evaluate Chebyshev polynomials
 *
 *  Copyright (C) 2007 by James A. Chappell (rlrrlrll@gmail.com)
 *
 *  Permission is hereby granted, free of charge, to any person
 *  obtaining a copy of this software and associated documentation
 *  files (the "Software"), to deal in the Software without
 *  restriction, including without limitation the rights to use,
 *  copy, modify, merge, publish, distribute, sublicense, and/or
 *  sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following
 *  condition:
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *  OTHER DEALINGS IN THE SOFTWARE.
 */
//=================================================================
/*
 * chebyshev.h:  Version 0.01
 * Created by James A. Chappell
 * Created 29 September 2005
 *
 * History:
 * 28-jul-2007  created
 */
//==============

#ifndef __CHEBYSHEV_H__
#define __CHEBYSHEV_H__
/*
 *	Function calculates Chebyshev Polynomials Tn(x)
 */

namespace chebyshev {

// n = 0
inline double T0(double)
{
    return 1.0 ;
}

// n = 1
inline double T1(double x)
{
    return x ;
}

// n = 2
inline double T2(double x)
{
    return (2.0 * x*x) - 1.0 ;
}

/*
 *	Tn(x)
 */
inline double Tn(unsigned int n, double x)
{
    if (n == 0)
    {
        return T0(x) ;
    }
    else if (n == 1)
    {
        return T1(x) ;
    }
    else if (n == 2)
    {
        return T2(x) ;
    }

    // We could simply do this:
    // return (2.0 * x * Tn(n - 1, x)) - Tn(n - 2, x) ;
    // but it could be slow for large n

    double tnm1(T2(x)) ;
    double tnm2(T1(x)) ;
    double tn(tnm1) ;

    for (unsigned int l = 3 ; l <= n ; l++)
    {
        tn = (2.0 * x * tnm1) - tnm2 ;
        tnm2 = tnm1;
        tnm1 = tn;
    }

    return tn ;
}
}
#endif

#endif // CHEBYSHEV_H
