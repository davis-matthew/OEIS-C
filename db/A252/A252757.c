/*
 * A252757  Permutation of natural numbers: a(1)=1, and for n>1, if n is k-th
 *          number whose largest prime factor is less than the square of its
 *          smallest prime factor [i.e., n = A251726(k)], a(n) = 2*a(k),
 *          otherwise, when n = A251727(k), a(n) = 1 + 2*a(k).
 * https://oeis.org/A252757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1. For n > 1 let P(n) be true iff the largest prime
 *            factor of n is less than the square of its smallest prime
 *            factor. Let k1(n) = #{ m < n | P(m) == true } // zero‑based
 *            count of P‑numbers k2(n) = #{ m < n | P(m) == false } //
 *            zero‑based count of non‑P numbers Then if P(n) a(n) = 2 *
 *            a(k1(...
 * Range:     u8 n <= 8, u16 n <= 78, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   property_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/property_of.h"

/* seq_t and idx_t are supplied by the build.  Allocate the table once
   and grow it as larger indices are requested. */
static seq_t *perm      = NULL;   /* 1‑based array of a(n) */
static idx_t   max_computed = 0;   /* highest n we have computed so far */

/* cumulative counts of numbers that satisfy or do not satisfy P(n) */
static idx_t countG1 = 0;          /* numbers with P(n)==true  (G1) */
static idx_t countG2 = 0;          /* numbers with P(n)==false (G2) */

/* ------------------------------------------------------------------ */
/*  Simple primality and factorisation helpers.  For the range of
    n we expect (up to about 10 000) trial division is fast enough.   */

/* ------------------------------------------------------------------ */
/*  Main sequence routine                                                  */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n < 1, but never used */

    /* grow the memoisation table if necessary */
    if (!perm || n > max_computed)
    {
        idx_t new_size = n + 1;                /* 1‑based array */
        if (!perm)
        {
            perm = (seq_t *)malloc(new_size * sizeof(seq_t));
            if (!perm) exit(1);
            perm[0] = 0;
            max_computed = 0;
            countG1 = 0;
            countG2 = 0;
        }
        else
        {
            perm = (seq_t *)realloc(perm, new_size * sizeof(seq_t));
            if (!perm) exit(1);
        }

        /* compute values from (max_computed+1) up to n included */
        for (idx_t i = max_computed + 1; i <= n; ++i)
        {
            if (i == 1)
            {
                perm[1] = 1;
                countG1 = 1;    /* 1 is a G1 number */
                countG2 = 0;
            }
            else
            {
                bool prop = property_of(i);
                if (prop)        /* G1 – use countG1 as position */
                {
                    idx_t pos = countG1;          /* 0‑based index of i within G1 */
                    perm[i] = perm[pos] * (seq_t)2;
                    ++countG1;
                }
                else             /* G2 – use (countG2+1) as position */
                {
                    idx_t pos = countG2;          /* 0‑based index of i within G2 */
                    perm[i] = perm[pos + 1] * (seq_t)2 + (seq_t)1;
                    ++countG2;
                }
            }
        }
        max_computed = n;
    }
    return perm[n];
}
