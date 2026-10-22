/*
 * A373399  For any number m, let m* be the bi-infinite string obtained by
 *          repetition of the binary expansion of m; a(n) is the least k such
 *          that the binary expansion of n appears in k*.
 * https://oeis.org/A373399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = minimal k (1 ≤ k ≤ n) such that the binary *
 *            representation of n appears as a contiguous substring of * the
 *            bi-infinite string obtained by repeating the binary *
 *            representation of k. By definition a(n) ≤ n, with equality * iff
 *            n is a power of two
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   n is the index (1‑based), returned value is of type seq_t.  */

#include <stdbool.h>

static inline idx_t bitlen_idx(idx_t v) {
    idx_t len = 0;
    while (v) {
        ++len;
        v >>= 1;
    }
    return len;
}

/* Return true iff binary(n) occurs in the infinite repetition of binary(k). */
static bool appears(idx_t n, idx_t k) {
    idx_t len_n = bitlen_idx(n);
    idx_t len_k = bitlen_idx(k);

    /* For each possible starting position in the cycle of k */
    for (idx_t t = 0; t < len_k; ++t) {
        bool ok = true;
        for (idx_t i = 0; i < len_n; ++i) {
            /* bit of n, most‑significant first */
            idx_t bit_n = (n >> (len_n - 1 - i)) & 1;
            /* corresponding bit in the infinite repetition of k */
            idx_t pos = (t + i) % len_k;
            idx_t bit_k = (k >> (len_k - 1 - pos)) & 1;
            if (bit_n != bit_k) {
                ok = false;
                break;
            }
        }
        if (ok) return true;
    }
    return false;
}

seq_t a(idx_t n) {
    /* n ≥ 1 */
    for (idx_t k = 1; k <= n; ++k) {
        if (appears(n, k))
            return (seq_t)k;
    }
    /* Fallback – should never be reached because a(n) always exists. */
    return (seq_t)n;
}
