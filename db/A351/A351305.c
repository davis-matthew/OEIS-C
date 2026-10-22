/*
 * A351305  a(n) = n^10 * Product_{p|n, p prime} (1 + 1/p^10).
 * https://oeis.org/A351305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = n^10 * Product_{p|n, p prime} (1 + 1/p^10)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 84, u128 n <= 7131,
 *            big any n
 * Helpers:   seq_pow_i
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_pow_i.h"

seq_t a(idx_t n)
{
    seq_t result = 1;
    uint64_t temp = (uint64_t)n;

    /* Trial division factorisation.  Good for n up to about 10 000. */
    for (uint64_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            seq_t p_big = (seq_t)p;
            seq_t term1 = seq_pow_i(p_big, 10 * e);          /* p^(10e)   */
            seq_t term2 = seq_pow_i(p_big, 10 * (e - 1));     /* p^(10(e‑1)) */
            result *= (term1 + term2);
        }
    }

    /* Handle a remaining prime factor > sqrt(original n). */
    if (temp > 1) {
        seq_t p_big = (seq_t)temp;
        seq_t term1 = seq_pow_i(p_big, 10);   /* p^10       */
        seq_t term2 = seq_pow_i(p_big, 0);    /* 1          */
        result *= (term1 + term2);
    }

    return result;
}
