/*
 * A391878  The number of vacuously transitive relations on an n-set.
 * https://oeis.org/A391878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n * Σ_{s=1..n} C(n,s) * (2^s - 1)^{n-s}
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"     /* provides seq_t pow2(idx_t) */
#include "../utils/big_pow.h"  /* provides seq_t big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                     /* a(0) = 1 */

    seq_t pow2_n = pow2(n);           /* 2^n */
    seq_t sum   = 0;                  /* Σ terms */
    seq_t binom = 1;                  /* C(n,0) */
    seq_t pow2_s = 1;                 /* 2^0 */

    for (idx_t s = 1; s <= n; ++s) {
        /* Update binomial coefficient C(n,s) = C(n,s-1) * (n - s + 1) / s */
        binom = binom * (n - s + 1) / s;

        /* Update 2^s */
        pow2_s = pow2_s * 2u;         /* safe for all bit‑widths (uses seq_t multiplication) */

        /* Compute (2^s - 1) */
        seq_t base = pow2_s - 1;

        /* Compute (2^s - 1)^{n-s} */
        seq_t power = big_pow(base, (seq_t)(n - s));

        /* term = C(n,s) * (2^s - 1)^{n-s} */
        seq_t term = binom * power;

        /* accumulate */
        sum = sum + term;
    }

    return sum * pow2_n;              /* a(n) = 2^n * sum */
}
