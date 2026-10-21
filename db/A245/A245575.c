/*
 * A245575  Number of ways of writing n as the sum of two quarter-squares (cf.
 *          A002620).
 * https://oeis.org/A245575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A245575 – number of ways of writing n as the sum of two quarter‑squares.
 *
 *  a(n) = #{ (x, y) : x = ⌊k²/4⌋, y = ⌊ℓ²/4⌋ for some k, ℓ ≥ 0 and x + y = n }
 *  Each pair is counted with order (x, y) and (y, x) considered distinct.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Gather every distinct quarter‑square value ≤ n. */
    size_t qs_len = 0;
    size_t qs_cap = 32;
    idx_t *qs = (idx_t*)malloc(sizeof(idx_t) * qs_cap);
    if (!qs) return 0;          /* allocation failure */

    for (idx_t m = 0;; ++m) {
        idx_t val = (m * m) >> 2;    /* floor(m²/4) */
        if (val > n) break;

        /* Keep only new distinct values. */
        if (qs_len == 0 || val != qs[qs_len - 1]) {
            if (qs_len == qs_cap) {           /* grow array */
                qs_cap <<= 1;
                idx_t *tmp = (idx_t*)realloc(qs, sizeof(idx_t) * qs_cap);
                if (!tmp) { free(qs); return 0; }  /* allocation failure */
                qs = tmp;
            }
            qs[qs_len++] = val;
        }
    }

    /* Count ordered pairs (x, y) such that x + y = n. */
    seq_t count = 0;
    for (size_t i = 0; i < qs_len; ++i) {
        idx_t xi = qs[i];
        idx_t target = n - xi;  /* target is always ≤ n */

        for (size_t j = 0; j < qs_len; ++j) {
            if (qs[j] == target)
                ++count;
        }
    }

    free(qs);
    return count;
}
