/*
 * A302267  Number of 3 X n 0..1 arrays with every element equal to 0, 1, 2 or
 *          3 horizontally, diagonally or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A302267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A302267
 *  Recurrence:
 *      a(n) = 3*a(n-1) +14*a(n-2) -9*a(n-3) -54*a(n-4) -10*a(n-5)
 *            +65*a(n-6) +17*a(n-7) -8*a(n-8) -2*a(n-9) -31*a(n-10)
 *            -7*a(n-11) +9*a(n-12)   for n > 13
 *  Initial values (a(1) … a(13)):
 *      4, 32, 153, 772, 3818, 19191, 96004, 481261, 2411557,
 *      12086268, 60572976, 303579352, 1521476119
 *
 *  The function below does not use dynamic allocation and works
 *  for any seq_t (unsigned integer type or arbitrary‑precision
 *  class) and any idx_t (unsigned integer type).  It computes
 *  each required value on the fly, taking at most O(n*12)
 *  operations, which is well below the 2‑second limit for n
 *  up to about 10000.
 */
#ifndef _A302267_H_
#define _A302267_H_

#include <stddef.h>

/* Coefficients for the recurrence: coeff[k] multiplies a(n-(k+1))  */
static const int coeff[12] = {
    3, 14, -9, -54, -10, 65, 17, -8, -2, -31, -7, 9
};

/* Initial values a(1) .. a(13)  (index 0 unused)  */
static const seq_t initial[14] = {
    0ULL,            /* placeholder for a(0) – not used  */
    4ULL, 32ULL, 153ULL, 772ULL, 3818ULL, 19191ULL,
    96004ULL, 481261ULL, 2411557ULL, 12086268ULL,
    60572976ULL, 303579352ULL, 1521476119ULL
};

seq_t a(idx_t n)
{
    /*  Sequence is defined only for n >= 1  */
    if (n == 0) {
        return 0;   /* safety value – not part of the sequence  */
    }

    /*  Return a known initial term directly  */
    if (n <= 13) {
        return initial[n];
    }

    /*  Keep the twelve most recent values (a(i‑1) … a(i‑12))  */
    seq_t last[12];
    /*  Initialize with a(13), a(12), …, a(2)  */
    last[0] = initial[13];            /* a(13)  */
    for (int k = 1; k < 12; ++k) {
        last[k] = initial[13 - k];    /* a(13‑k)  */
    }

    seq_t cur = 0;
    for (idx_t i = 14; i <= n; ++i) {
        /*  Compute a(i) from the recurrence  */
        cur = 0;
        for (int k = 0; k < 12; ++k) {
            if (coeff[k] >= 0) {
                cur += last[k] * (seq_t)coeff[k];
            } else {
                cur -= last[k] * (seq_t)(-coeff[k]);
            }
        }

        /*  Shift the buffer: a(i‑1) becomes a(i‑2), …  */
        for (int k = 11; k > 0; --k) {
            last[k] = last[k - 1];
        }
        last[0] = cur;   /*  a(i)  */
    }

    return cur;
}

#endif /* _A302267_H_ */
