/*
 * A265236  Number of solutions to the equation A x B = C, where A, B and C
 *          are nonnegative numbers appearing as (contiguous) substrings of
 *          the binary representation of n.
 * https://oeis.org/A265236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1471, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binary_find
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A265236
 * a(n) = #{(A,B,C) | A·B = C,  A,B,C are (contiguous) substrings of binary(n)}
 * with substrings allowed to be zero.
 *
 * Implemented by enumerating all distinct substrings of the binary
 * representation, then counting ordered pairs (A,B) whose product equals
 * another substring.  All intermediate numbers fit in 64 bits,
 * so only 64‑bit integer arithmetic is used.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "../utils/binary_find.h"

/* compute floor(log2(v)) for v>0; returns 0 when v==0  */
static inline idx_t floor_log2_uint64(idx_t v) {
    idx_t r = 0;
    while (v >>= 1) ++r;
    return r;
}

static int cmp_uint64(const void *p, const void *q) {
    uint64_t a = *(const uint64_t *)p;
    uint64_t b = *(const uint64_t *)q;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

seq_t a(idx_t n) {
    /* maximum number of bits for any 64‑bit n is 64; 64×65/2 = 2080 substrings */
    enum { MAX_SUB = 2080 };
    uint64_t all[MAX_SUB];
    size_t all_cnt = 0;

    /* determine bit length of n */
    idx_t len = (n == 0) ? 1 : floor_log2_uint64(n) + 1;

    /* extract bits into array (MSB first) */
    uint8_t bits[64];
    for (idx_t i = 0; i < len; ++i) {
        bits[i] = (uint8_t)((n >> (len - 1 - i)) & 1ULL);
    }

    /* enumerate all contiguous substrings */
    for (idx_t i = 0; i < len; ++i) {
        uint64_t val = 0;
        for (idx_t j = i; j < len; ++j) {
            val = (val << 1) | bits[j];
            all[all_cnt++] = val;
        }
    }

    /* sort and remove duplicates */
    qsort(all, all_cnt, sizeof(uint64_t), cmp_uint64);
    uint64_t uniq[MAX_SUB];
    size_t uniq_cnt = 0;
    for (size_t k = 0; k < all_cnt; ++k) {
        if (k == 0 || all[k] != all[k - 1]) {
            uniq[uniq_cnt++] = all[k];
        }
    }

    /* maximum possible substring value */
    uint64_t max_val = (len == 64) ? UINT64_MAX : ((1ULL << len) - 1ULL);

    /* count valid (A,B,C) triples */
    uint64_t cnt = 0;
    for (size_t i = 0; i < uniq_cnt; ++i) {
        uint64_t a_val = uniq[i];
        for (size_t j = 0; j < uniq_cnt; ++j) {
            uint64_t b_val = uniq[j];
            uint64_t prod;
            if (a_val == 0 || b_val == 0) {
                prod = 0;
            } else {
                /* if product would exceed max_val, it cannot be a substring */
                if (a_val > max_val / b_val) continue;
                prod = a_val * b_val;         /* safe: prod <= max_val <= 2^64-1 */
            }
            if (binary_find(prod, uniq, uniq_cnt)) ++cnt;
        }
    }

    return (seq_t)cnt;
}
