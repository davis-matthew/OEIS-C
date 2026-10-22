/*
 * A340760  Number of partitions of n into 4 parts whose largest 3 parts have
 *          the same parity.
 * https://oeis.org/A340760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A340760: Number of partitions of n into 4 parts whose
   largest 3 parts have the same parity.  */
#include <stdint.h>

/* External types */
extern seq_t a(idx_t n);

/*---------------------------------------------------------------------------*/
/* The algorithm works by enumerating the smallest part c (the third
   largest part) and, for each c, counting all triples (a,b,c) with
   a>=b>=c and with a,b having the same parity as c, such that the
   remaining part d = n - a - b - c satisfies 1 <= d <= c.
   The problem reduces to counting integer solutions of
       a + b = S   with   L <= S <= U   and   a,b >= c
   where
       L = max(2c, n-2c)          // from a+b >= 2c  and  a+b >= n-2c
       U = n-1-c                   // from a+b+c <= n-1
   Only a,b with the same parity as c are allowed.
   The solution set is summarized by the following arithmetic
   construction:

   Let p = c % 2   (parity of c)
   Let C = (c - p)/2     // integer part of c/2
   Write a = 2A + p , b = 2B + p
   Then   A + B = t ,   L' <= t <= U'  where
       L' = ceil( (L - 2p) / 2 )
       U' = floor( (U - 2p) / 2 )
   For a given t, the number of pairs (A,B) with
       A >= B >= C   and   A+B = t
   equals  floor(t/2) - C + 1   if floor(t/2) >= C, otherwise 0.

   Summing this quantity over all admissible t gives the number of
   valid (a,b,c) triples.  Finally we sum over all possible c.
--------------------------------------------------------------------------*/

seq_t a(idx_t n) {
    /* Offset 0: for n < 4 no 4-part partition exists */
    if (n < 4)
        return 0;

    seq_t total = 0;

    /* Smallest part c can be at most floor((n-1)/3) */
    idx_t c_max = (n - 1) / 3;
    for (idx_t c = 1; c <= c_max; ++c) {
        /* Parity of c */
        idx_t p = c & 1ul;
        /* Integer part of c/2 */
        idx_t C = (c - p) >> 1;

        /* Compute L = max(2c, n-2c) */
        idx_t L = 2 * c;
        if (n > 2 * c) {
            idx_t tmp = n - 2 * c;
            if (tmp > L)
                L = tmp;
        }

        /* Compute U = n-1-c */
        idx_t U = n - 1 - c;
        if (L > U)
            continue;  /* no solution for this c */

        /* Transform to t-range */
        idx_t Lprime = (L - 2 * p + 1) >> 1;   /* ceil division */
        idx_t Uprime = (U - 2 * p) >> 1;        /* floor division */
        if (Lprime > Uprime)
            continue;

        for (idx_t t = Lprime; t <= Uprime; ++t) {
            idx_t Bmax = t >> 1; /* floor(t/2) */
            if (Bmax < C)
                continue;
            total += (seq_t)(Bmax - C + 1);
        }
    }
    return total;
}
