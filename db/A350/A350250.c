/*
 * A350250  Numbers k such that the k-th composition in standard order is a
 *          non-alternating permutation of an initial interval of positive
 *          integers.
 * https://oeis.org/A350250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: enumerates nonnegative integers k and tests whether the
   k‑th composition (in graded reverse‑lexicographic order) is a
   non‑alternating permutation of {1,…,m}.  The n‑th term a(n) is the
   n‑th such integer k, with offset 1. */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

static bool is_valid_k(uint64_t k)
{
    if (k == 0) return false;
    uint64_t parts[64];
    size_t m = 0;
    uint64_t pos = 1;
    uint64_t prev = 0;
    uint64_t sum = 0;
    uint64_t tmp = k;
    while (tmp != 0) {
        if (tmp & 1ULL) {
            uint64_t diff = pos - prev;
            parts[m++] = diff;
            sum += diff;
            prev = pos;
        }
        tmp >>= 1;
        ++pos;
    }
    if (m == 0) return false;
    uint64_t tri = (uint64_t)m * (m + 1) / 2;
    if (sum != tri) return false;
    bool seen[65] = {false};
    for (size_t i = 0; i < m; ++i) {
        uint64_t p = parts[i];
        if (p < 1 || p > m) return false;
        if (seen[p]) return false;
        seen[p] = true;
    }
    if (m < 3) return false;   /* all length‑<3 permutations are alternating */
    bool up = parts[0] < parts[1];
    for (size_t i = 1; i < m - 1; ++i) {
        bool cur = parts[i] < parts[i + 1];
        if (cur == up) return true;   /* found a non‑alternating step */
        up = cur;
    }
    return false;  /* fully alternating */
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    uint64_t k = 0;
    while (1) {
        if (is_valid_k(k)) {
            ++count;
            if (count == n) {
                return (seq_t)k;
            }
        }
        ++k;
    }
}
