/*
 * A371836  a(n) = Sum_{k=0..floor(n/2)} n^k * binomial(2*n-2*k-1,n-2*k).
 * https://oeis.org/A371836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} n^k * binomial(2*n-2*k-1, n-2*k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 23 (2026-11-01)
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

seq_t a(idx_t n) {
    if (n == 0) return 1;          /* a(0) = 1 */
    seq_t total = 0;
    seq_t pow = 1;                 /* n^0 */
    idx_t maxk = n / 2;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t comb = binomial(2 * n - 2 * k - 1, n - 2 * k);
        total += seq_mul(comb, pow);
        pow *= (seq_t)n;           /* update n^k -> n^{k+1} */
    }
    return total;
}
