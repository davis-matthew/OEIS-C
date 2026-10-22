/*
 * A342294  a(n) = Sum_{k = 0..n} binomial(n,k)^11.
 * https://oeis.org/A342294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k = 0..n} binomial(n,k)^11
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t bin = 1; /* current binomial coefficient binomial(n,k) */

    for (idx_t k = 0; k <= n; ++k) {
        sum += pow_int(bin, 11);
        if (k == n) break;  /* no further update needed */

        /* bin(n,k+1) = bin(n,k) * (n-k) / (k+1) */
        bin = bin * ((seq_t)(n - k)) / ((seq_t)(k + 1));
    }

    return sum;
}
