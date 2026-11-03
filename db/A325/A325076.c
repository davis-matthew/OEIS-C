/*
 * A325076  Prime numbers congruent to 1, 16 or 22 modulo 39 neither
 *          representable by x^2 + x*y + 10*y^2 nor by x^2 + x*y + 127*y^2.
 * https://oeis.org/A325076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 36, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-03)
 * Helpers:   ipow_i32, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A325076: primes p ≡ 1, 16, 22 (mod 39)
   that are not representable by x² + x*y + 10*y² nor by
   x² + x*y + 127*y².  The algorithm generates primes
   incrementally and tests the representability condition. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/ipow_i32.h"
#include "../utils/next_prime.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

static bool represent_square(seq_t p, seq_t Dprime) {
    seq_t fourp = (seq_t)4 * p;
    seq_t y_max_sq = fourp / Dprime;
    seq_t y_max = floor_sqrt(y_max_sq);
    for (seq_t y = 0; y <= y_max; ++y) {
        seq_t ysq = y * y;
        seq_t S   = fourp - Dprime * ysq;
        seq_t s   = floor_sqrt(S);
        if (s * s == S) return true;
    }
    return false;
}

seq_t a(idx_t n) {
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;
    static size_t seq_len = 0;
    static uint64_t cur_prime = 0;

    if (seq == NULL) {
        seq_cap = 1024;
        seq = mem_new(seq_t, seq_cap);
    }

    while (seq_len < (size_t)n) {
        cur_prime = next_prime(cur_prime);
        uint64_t r = cur_prime % 39;
        if (r != 1 && r != 16 && r != 22) continue;

        seq_t p = (seq_t)cur_prime;
        if (!represent_square(p, (seq_t)39) &&
            !represent_square(p, (seq_t)507)) {
            if (seq_len == seq_cap) {
                seq_cap *= 2;
                seq = mem_resize(seq_t, seq, seq_cap);
            }
            seq[seq_len++] = p;
        }
    }
    return seq[n - 1];
}
