/*
 * A379085  a(n) = Sum_{k=0..floor(n/3)} binomial(2*n+k-1,k) *
 *          binomial(2*n+k,n-3*k).
 * https://oeis.org/A379085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(2*n+k-1,k) *
 *            binomial(2*n+k,n-3*k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 24 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t sum = 0;
    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t c1 = binomial(2*n + k - 1, k);
        seq_t c2 = binomial(2*n + k, n - 3*k);
        seq_t prod = seq_mul(c1, c2);
        sum += prod;
    }
    return sum;
}
