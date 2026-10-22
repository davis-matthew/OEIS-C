/*
 * A384229  Numerators of the expected number of steps to hit the opposite
 *          corner by simple random walk on the n-cube.
 * https://oeis.org/A384229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator( n * Σ_{k=1..n} 2^(k-1)/k )
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_gcd.h"


seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* safety – the sequence is defined for n ≥ 1 */

    seq_t S_num = 0;   /* numerator of the partial sum                */
    seq_t S_den = 1;   /* denominator of the partial sum                */
    seq_t power = 1;   /* current value of 2^(k‑1), starts with 1 for k=1 */

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t term_num = power;          /* 2^(k‑1)                */
        seq_t term_den = (seq_t)k;        /* k                       */

        seq_t new_num = S_num * term_den + term_num * S_den;
        seq_t new_den = S_den * term_den;

        seq_t g = seq_gcd(new_num, new_den);
        S_num = new_num / g;
        S_den = new_den / g;

        power *= (seq_t)2;                /* advance to 2^k for next k   */
    }

    seq_t num = S_num * n;                /* numerator of E(n) * n       */
    seq_t den = S_den;                    /* denominator unchanged        */

    seq_t g = seq_gcd(num, den);
    return num / g;                       /* return the reduced numerator */
}
