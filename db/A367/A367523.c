/*
 * A367523  The number of ways of tiling the n X n grid up to the symmetries
 *          of the square by a tile that is fixed under 90-degree rotations,
 *          but not reflections.
 * https://oeis.org/A367523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(2m‑1) = 2^(m^2‑4m‑2) * ( 2^(3m+1) + 2^(m^2+2m) +
 *            8^(m^2) ) a(2m) = 2^(m^2‑3) * ( 2 + 3·2^(m^2) + 8^(m^2) )
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not part of the sequence but harmless */

    idx_t m;
    int64_t exp;
    seq_t t1, t2, t3, sum, res;

    if (n & 1U) {                       /* odd n = 2m‑1 */
        m = (n + 1) / 2;

        t1 = pow2((idx_t)(3 * m + 1));          /* 2^(3m+1)          */
        t2 = pow2((idx_t)(m * m + 2 * m));      /* 2^(m^2+2m)        */
        t3 = pow2((idx_t)(3 * m * m));          /* 8^(m^2) = 2^(3m^2) */

        sum = t1 + t2 + t3;

        exp = (int64_t)(m * m) - 4 * (int64_t)m - 2;
        if (exp >= 0)
            res = sum << (idx_t)exp;           /* multiply by 2^exp   */
        else
            res = sum >> (idx_t)(-exp);        /* divide by 2^(-exp)  */
    } else {                               /* even n = 2m     */
        m = n / 2;

        t1 = pow2((idx_t)(m * m));          /* 2^(m^2)          */
        t2 = pow2((idx_t)(3 * m * m));      /* 8^(m^2) = 2^(3m^2) */

        sum = (seq_t)2 + (seq_t)3 * t1 + t2;

        exp = (int64_t)(m * m) - 3;
        if (exp >= 0)
            res = sum << (idx_t)exp;         /* multiply by 2^exp   */
        else
            res = sum >> (idx_t)(-exp);      /* divide by 2^(-exp)  */
    }

    return res;
}
