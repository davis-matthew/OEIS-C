/*
 * A373878  Row products of A317617.
 * https://oeis.org/A373878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 149 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A373878: Row products of A317617
   Formula:
   a(n) = ((n^3 + n)/2)^n                      for n even
   a(n) = Pochhammer((1 + n^3)/2, n)           for n odd
   with a(0) = 1, a(1) = 1.   */

/* Note: The helper functions seq_pow and seq_mul are defined in
   "../utils/seq_pow.h" and "../utils/seq_mul.h".  These provide
   big‑integer multiplication and exponentiation for the type seq_t. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n) {
    /* Handle the offset explicitly. */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Even n: a(n) = ( (n^3 + n)/2 )^n */
    if ((n & 1) == 0) {
        /* Compute base = n * (n^2 + 1) / 2  */
        seq_t nn = (seq_t)n;
        seq_t base = nn * (nn * nn + (seq_t)1);
        base = base / (seq_t)2;
        return ipow_idx(base, n);
    }

    /* Odd n: a(n) = Pochhammer((1 + n^3)/2, n) = prod_{k=0}^{n-1} (((1+n^3)/2)+k) */
    seq_t nn = (seq_t)n;
    seq_t base = ((nn * nn * nn) + (seq_t)1) / (seq_t)2;

    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        seq_t term = base + (seq_t)i;
        result = seq_mul(result, term);
    }
    return result;
}
