/*
 * A354766  1/4 of the total number of integral quadruples with sum = n and
 *          sum of squares = n^2.
 * https://oeis.org/A354766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = (number of integral quadruples (h,i,j,k) with h+i+j+k = n
 *            and h²+i²+j²+k² = n²) / 4
 * Range:     u8 n <= 134, u16 n <= 650, u32 n <= 650, u64 n <= 650,
 *            u128 n <= 650, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation counts ordered tuples (h,i,j,k) by
   transforming the problem to counting integer quadruples
   (u,d,v) with u = a+b, d = a−b, v = n−u.  For each such
   (u,d) we decide whether a pair (c,d) exists using a
   discriminant test.  The counts are accumulated and
   finally divided by 4.  */

#include <math.h>
#include <stdbool.h>

/* external types supplied by the build */
extern seq_t a(idx_t n);

/* Returns the nth term of A354766 for n >= 1 */
seq_t a(idx_t n)
{
    /* Constants for bounding u */
    const double u_min_coeff = (1.0 - sqrt(3.0)) / 2.0;
    const double u_max_coeff = (1.0 + sqrt(3.0)) / 2.0;

    /* n is small enough that 64‑bit arithmetic is safe for all
       intermediate calculations.  Use 128‑bit signed integers for
       the squares to avoid overflow.  */
    __int128 nsq = (__int128)n * (__int128)n;

    int64_t n64 = (int64_t)n;                /* for arithmetic loops */
    int64_t u_min = (int64_t)floor(u_min_coeff * (double)n);
    int64_t u_max = (int64_t)ceil(u_max_coeff * (double)n);

    /* Count all ordered quadruples */
    unsigned long long total = 0ULL;

    for (int64_t u = u_min; u <= u_max; ++u) {
        /* Compute bound for |d|:    d² ≤ n² + 2nu – 2u² */
        long double bound = (long double)nsq
                           + 2.0L * (long double)n64 * (long double)u
                           - 2.0L * (long double)u * (long double)u;
        if (bound < 0.0L)
            continue;

        int64_t dmax = (int64_t)floor(sqrt((double)bound) + 1e-12);

        /* d must have the same parity as u */
        int64_t d_start = -dmax;
        if ( ((d_start ^ u) & 1LL) != 0 )
            d_start += 1;                      /* adjust to correct parity */

        for (int64_t d = d_start; d <= dmax; d += 2) {
            __int128 D = nsq + (__int128)2 * ( (__int128)n64 * u )
                           - (__int128)2 * (__int128)u * u
                           - (__int128)d * d;
            if (D < 0)
                continue;

            /* Integer square root of D */
            unsigned long long Dull = (unsigned long long)D;
            unsigned long long sD = (unsigned long long)(sqrt((double)Dull) + 0.5);
            while (sD * sD > Dull) --sD;
            while ((sD + 1) * (sD + 1) <= Dull) ++sD;
            if (sD * sD != Dull)
                continue;

            /* Parity check: v = n−u and sD must have same parity */
            int v_parity = (int)((n64 - u) & 1LL);
            if ( (sD & 1ULL) != v_parity )
                continue;

            /* Number of ordered pairs (c,d) for this triple */
            total += (sD == 0) ? 1ULL : 2ULL;
        }
    }

    /* Divide by 4 as required by the sequence definition */
    return (seq_t)(total >> 2);
}
