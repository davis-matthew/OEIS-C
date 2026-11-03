/*
 * A334966  Numbers k such that the k-th composition in standard order (row k
 *          of A066099) has weakly decreasing non-adjacent parts.
 * https://oeis.org/A334966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A334966.
   a(n) is the nth integer k such that the k‑th composition in standard
   order (graded reverse‑lexicographic) has weakly decreasing
   non‑adjacent parts.
   Implementation: enumerate k, build its composition from the binary
   representation of k, reverse the parts, and test the
   non‑adjacent weakly decreasing condition. */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

static bool valid_k(idx_t k)
{
    /* Build composition parts from k. */
    uint64_t parts[64];
    size_t len = 0;
    uint64_t prev_pos = 0;
    uint64_t kk = k;
    uint64_t bit = 0;
    while (kk) {
        if (kk & 1ULL) {
            uint64_t cur_pos = bit + 1;
            parts[len++] = cur_pos - prev_pos;
            prev_pos = cur_pos;
        }
        kk >>= 1;
        ++bit;
    }
    /* Reverse parts to obtain the standard‑order composition. */
    for (size_t i = 0; i + i < len; ++i) {
        uint64_t tmp = parts[i];
        parts[i] = parts[len - 1 - i];
        parts[len - 1 - i] = tmp;
    }
    /* Check weakly decreasing non‑adjacent parts. */
    for (size_t i = 0; i + 1 < len; ++i) {
        for (size_t j = i + 2; j < len; ++j) {
            if (parts[i] < parts[j]) {
                return false;
            }
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t k = 0;; ++k) {
        if (valid_k(k)) {
            ++count;
            if (count == n) {
                return (seq_t)k;
            }
        }
    }
}
