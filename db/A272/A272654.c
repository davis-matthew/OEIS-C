/*
 * A272654  Binary words beginning with 1 which are abelian squares.
 * https://oeis.org/A272654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) enumerates binary words of even length that start with 1 *
 *            and are abelian squares (two halves have the same multiset of
 *            bits), * listed in lexicographic order grouped by increasing
 *            string length
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 175, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _binomial_ull, _count_strings_k, _gcd_ull
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/_binomial_ull.h"
#include "../utils/_count_strings_k.h"
#include "../utils/_gcd_ull.h"

/* Main sequence function */
seq_t a(idx_t n) {
    /* locate the block (length) that contains the n‑th word */
    unsigned long long cumulative = 0;
    idx_t k = 1;
    unsigned long long count_k = 0;
    for (;;) {
        count_k = _count_strings_k(k);
        if (cumulative + count_k >= n) break;
        cumulative += count_k;
        ++k;
    }
    unsigned long long offset = n - cumulative;               /* 1‑based rank within this length */

    /* find the first half u */
    idx_t start = (idx_t)1 << (k - 1);
    idx_t end   = (idx_t)1 << k;
    idx_t chosen_u = 0;
    idx_t ones_u = 0;
    unsigned long long off = offset;
    for (idx_t u = start; u < end; ++u) {
        idx_t ones = __builtin_popcountll(u);
        unsigned long long cnt = _binomial_ull(k, ones);
        if (off > cnt) {
            off -= cnt;
        } else {
            chosen_u = u;
            ones_u   = ones;
            break;
        }
    }

    /* build the bits of u and v into an array */
    unsigned char bits[128] = {0};
    size_t idx = 0;

    /* first half (u) */
    for (idx_t i = k; i > 0; --i) {
        bits[idx++] = (unsigned char)((chosen_u >> (i - 1)) & 1ULL);
    }

    /* second half (v) – unranking the lexicographic permutation */
    idx_t rem_ones  = ones_u;
    idx_t rem_zeros = k - ones_u;
    unsigned long long rank = off;              /* 1‑based rank within permutations */

    for (idx_t pos = 0; pos < k; ++pos) {
        unsigned long long perm0 = 0;
        if (rem_zeros > 0) {
            perm0 = _binomial_ull(rem_ones + rem_zeros - 1, rem_ones);
        }
        if (rank <= perm0) {
            bits[idx++] = 0;
            --rem_zeros;
        } else {
            rank -= perm0;
            bits[idx++] = 1;
            --rem_ones;
        }
    }

    /* convert the digit array into a seq_t decimal integer */
    seq_t value = 0;
    for (idx_t i = 0; i < 2 * k; ++i) {
        value = value * 10 + (seq_t)bits[i];
    }
    return value;
}
