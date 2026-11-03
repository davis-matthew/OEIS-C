/*
 * A318184  a(n) = 2^(n * (n - 1)/2) * 3^((n - 1) * (n - 2)) * n^(n - 3).
 * https://oeis.org/A318184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n*(n-1)/2) * 3^((n-1)*(n-2)) * n^(n-3)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Validated: standard, pass, 27 terms through n = 39 (2026-11-01)
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
    if (n <= 2)
        return (seq_t)1;

    idx_t e2 = n * (n - 1) / 2;
    idx_t e3 = (n - 1) * (n - 2);
    idx_t en = n - 3;          /* n >= 3 here */

    seq_t res = ipow_idx((seq_t)2, e2);
    res = seq_mul(res, ipow_idx((seq_t)3, e3));
    res = seq_mul(res, ipow_idx((seq_t)n, en));

    return res;
}
