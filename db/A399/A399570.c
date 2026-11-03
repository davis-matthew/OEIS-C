/*
 * A399570  a(n) = Product_{k=1..n} (2*k-1)^k.
 * https://oeis.org/A399570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} (2*k-1)^k
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 11 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    if (n < 1)   /* offset starts at 1; below this is undefined */
        return 0;

    seq_t result = 1;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t base = (seq_t)(2*k - 1);   /* odd number (2k-1) */
        seq_t pow_val = ipow_idx(base, k);/* (2k-1)^k */
        result = seq_mul(result, pow_val);
    }
    return result;
}
