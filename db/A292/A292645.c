/*
 * A292645  Rank of (6+r)*n when all the numbers (6-r)*j and (6+r)*k, where r
 *          = sqrt(2), j>=1, k>=1, are jointly ranked.
 * https://oeis.org/A292645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 97, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 * Formula: a(n) = n + floor( ((19 + 6*sqrt(2)) / 17) * n )
 * The sequence floor(n*α) where α = (19+6*sqrt(2))/17 is a Beatty sequence.
 * It can be generated with the recurrence
 *   a_1 = 1
 *   a_{k+1} = a_k + d_k          where d_k = 2 if (19 + p_k) + (6 + q_k)*sqrt(2) >= 34,
 *                                            otherwise d_k = 1
 *   p_{k+1} = 19 + p_k - 17*d_k
 *   q_{k+1} = 6 + q_k
 * Here p_k and q_k encode the fractional part of k*α as (p_k + q_k*sqrt(2))/17.
 * The test (19+p)+(6+q)*sqrt(2) >= 34 is performed without floating point
 * by squaring both sides: 2*(6+q)^2 >= (34-(19+p))^2 when the right side is positive.
 * After computing a_k = floor(k*α) we return a(n) = n + a_k.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used, but keep for safety */

    /* floor(α) = 1 */
    seq_t floor_val = 1;

    /* p and q are integers that satisfy
       fractional part of k*α = (p + q*sqrt(2)) / 17   */
    int64_t p = 2;   /* 19*1 - 17*1 */
    int64_t q = 6;   /* 6*1          */

    for (idx_t k = 2; k <= n; ++k)
    {
        /* test if (19 + p) + (6 + q)*sqrt(2) >= 34   */
        int64_t A = 19 + p;
        int64_t B = 6 + q;                /* B > 0 for all k */
        int d; /* 1 or 2 */
        int64_t C_minus_A = 34 - A;       /* difference from 34 */

        if (C_minus_A <= 0)
            d = 2;                       /* guaranteed to be >= 2 */
        else
        {
            /* compare 2*B^2 ? (C_minus_A)^2   */
            __int128 left  = (__int128)B * B * 2;
            __int128 right = (__int128)C_minus_A * C_minus_A;
            d = (left >= right) ? 2 : 1;
        }

        floor_val += (seq_t)d;

        /* update p and q for next step */
        p = 19 + p - 17 * d;
        q = 6 + q;
    }

    /* a(n) = n + floor(n*α) */
    return (seq_t)n + floor_val;
}
