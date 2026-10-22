/*
 * A358821  a(n) is the largest square dividing n^2 - 1.
 * https://oeis.org/A358821
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest square dividing n^2 – 1 (OEIS A358821)
 * Range:     u8 n <= 16, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    /* The sequence is defined for n >= 2 */
    if (n < 2) return 0;

    /* Compute m = n^2 – 1 as a seq_t */
    seq_t m = (seq_t)n * (seq_t)n - 1;

    /* Factor m using the provided factor_u64 utility */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)m, primes, exps, &cnt);

    /* Build the largest square divisor from the prime factorisation */
    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p   = (seq_t)primes[i];
        seq_t p2  = p * p;            /* p squared */
        size_t half = exps[i] / 2;    /* floor(e/2) */

        seq_t sq = 1;
        for (size_t k = 0; k < half; ++k)
            sq *= p2;                 /* multiply by p^2 each time */

        result *= sq;
    }

    return result;
}
