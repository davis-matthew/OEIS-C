/*
 * A263655  Table T(m, n) of number of circular binary strings with m ones and
 *          n zeros without zigzags, read by antidiagonals (see reference for
 *          precise definition).
 * https://oeis.org/A263655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn
 * Formula:   not stated
 * Range:     u8 n <= 141, u16 n <= 417, u32 n <= 819, u64 n <= 819,
 *            u128 n <= 819, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:  a(n) = T(m,n) read by antidiagonals, where
 * 
 *     T(m,n) = Σ_{k≥0} U(m,k)*U(n,k)  – 2*V(m,k)*V(n,k)*(-1)^k
 * 
 *     U(r,k) = binomial(r‑k+2⋅⌊k/3⌋, ⌊k/3⌋)
 *     V(r,k) = binomial(r‑⌈k/2⌉‑1, ⌊k/2⌋)
 * 
 * The sequence a(n) is formed by enumerating the table T(m,n) (m
 *  = number of ones, n = number of zeros) antidiagonally with offset
 *  0:  a(0)=0, a(1)=1, a(2)=1, a(3)=1, a(4)=0, …
 * 
 * Implementation uses an on‑the‑fly calculation of T(m,n); the only
 * special case required is a(0)=0 (T(0,0) is defined to be 0).
 *********************************************************************/

#include <stdint.h>
#include <stddef.h>
#include "../utils/binom.h"   /* seq_t binom(idx_t a, idx_t b);  */

/* Compute T(ones,zeros) with the given formula. */
static inline seq_t calc_T(idx_t ones, idx_t zeros)
{
    seq_t sum = 0;

    /* Upper bound for k: 3 * max(ones,zeros) + 5 is sufficient. */
    idx_t m = (ones > zeros ? ones : zeros);
    idx_t bound = m * 3 + 5;

    for (idx_t k = 0; k <= bound; ++k) {
        idx_t f3 = k / 3;                  /* ⌊k/3⌋ */

        /* U(ones,k) */
        int64_t topO = (int64_t)ones - (int64_t)k + 2 * (int64_t)f3;
        seq_t Uo = 0;
        if (topO >= 0 && (idx_t)topO >= f3)
            Uo = binom((idx_t)topO, f3);

        /* U(zeros,k) */
        int64_t topZ = (int64_t)zeros - (int64_t)k + 2 * (int64_t)f3;
        seq_t Uz = 0;
        if (topZ >= 0 && (idx_t)topZ >= f3)
            Uz = binom((idx_t)topZ, f3);

        sum += Uz * Uo;                    /* first term of the summand */

        /* V(ones,k) */
        idx_t ceil_k2 = (k + 1) / 2;       /* ⌈k/2⌉ */
        idx_t floor_k2 = k / 2;            /* ⌊k/2⌋ */
        int64_t topVO = (int64_t)ones - (int64_t)ceil_k2 - 1;
        seq_t Vo = 0;
        if (topVO >= 0 && (idx_t)topVO >= floor_k2)
            Vo = binom((idx_t)topVO, floor_k2);

        /* V(zeros,k) */
        int64_t topVZ = (int64_t)zeros - (int64_t)ceil_k2 - 1;
        seq_t Vz = 0;
        if (topVZ >= 0 && (idx_t)topVZ >= floor_k2)
            Vz = binom((idx_t)topVZ, floor_k2);

        seq_t termV = Vz * Vo;             /* V*V part */
        termV <<= 1;                       /* multiply by 2 */

        if (k & 1U)                        /* odd k: add 2*V*V   */
            sum += termV;
        else                               /* even k: subtract   */
            sum -= termV;
    }
    return sum;
}

/* Public sequence routine */
seq_t a(idx_t n)
{
    /* Special case: T(0,0) is defined as 0 in the table. */
    if (n == 0)
        return 0;

    /* Locate which antidiagonal this index falls into. */
    idx_t d = 0;
    while (d * (d + 1) / 2 <= n)
        ++d;
    --d;                                      /* largest d with first index ≤ n */

    idx_t first_in_diag = d * (d + 1) / 2;
    idx_t zeros = n - first_in_diag;          /* number of zeros (row)   */
    idx_t ones  = d - zeros;                  /* number of ones  (col)   */

    return calc_T(ones, zeros);
}
