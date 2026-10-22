/*
 * A368810  a(n) = numerator of Sum_{i=1..n} Sum_{j=1..n} (1/i + 1/j).
 * https://oeis.org/A368810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of 2*n*H_n (H_n = Σ_{k=1}^{n} 1/k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)                     /* sequence is 1‑indexed; 0 is unused */
        return 0;

    seq_t num = 0;                  /* current numerator of the harmonic sum */
    seq_t den = 1;                  /* current denominator   */

    /* Compute H_n as a reduced fraction num/den */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t k_seq = (seq_t)k;
        seq_t new_num = num * k_seq + den;
        seq_t new_den = den * k_seq;
        seq_t g = gcd(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
    }

    /* Multiply by 2*n and reduce once more */
    seq_t two_n = (seq_t)2 * (seq_t)n;
    seq_t final_num = num * two_n;
    seq_t g2 = gcd(final_num, den);

    return final_num / g2;
}
