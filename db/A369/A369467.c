/*
 * A369467  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A369465(i) = A369465(j) and A369465(A163511(i)) =
 *          A369465(A163511(j)), for all i, j >= 1.
 * https://oeis.org/A369467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of the pairs
 *            [A369465(n), A369465(A163511(n))]; computed by ranking keys in
 *            order of first appearance
 * Range:     u8 n <= 524, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 114 terms through n = 9778 (2026-11-10)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

static uint64_t A369467_deriv_odd(uint64_t m)
{
    if (m == 0) return 0;
    while ((m & 1ULL) == 0ULL) m >>= 1;
    if (m <= 1ULL) return 0;
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64(m, pr, ex, &cnt);
    uint64_t r = 0;
    for (size_t i = 0; i < cnt; ++i)
        r += (m / pr[i]) * ex[i];
    return r;
}

static uint64_t A369467_a163511(uint64_t n)
{
    static const uint64_t pt[20] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71};
    if (n == 0) return 1;
    int cnt = 0;
    uint64_t pos[64];
    uint64_t bitpos = 1, t = n;
    while (t) {
        if (t & 1ULL) pos[cnt++] = bitpos;
        ++bitpos;
        t >>= 1;
    }
    if (cnt > 20) return 1;
    uint64_t res = pt[cnt - 1];
    uint64_t prev = 0;
    for (int i = 0; i < cnt; ++i) {
        uint64_t e = pos[i] - prev - 1;
        for (uint64_t k = 0; k < e; ++k) res *= pt[i];
        prev = pos[i];
    }
    return res;
}

static uint64_t *A369467_k1 = NULL;
static uint64_t *A369467_k2 = NULL;
static uint64_t *A369467_res = NULL;
static size_t A369467_done = 0;
static size_t A369467_cap = 0;
static size_t A369467_distinct = 0;

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    if (N > A369467_done) {
        if (N > A369467_cap) {
            size_t nc = N * 2 + 16;
            A369467_k1 = (uint64_t *)realloc(A369467_k1, (nc + 1) * sizeof(uint64_t));
            A369467_k2 = (uint64_t *)realloc(A369467_k2, (nc + 1) * sizeof(uint64_t));
            A369467_res = (uint64_t *)realloc(A369467_res, (nc + 1) * sizeof(uint64_t));
            A369467_cap = nc;
        }
        /* distinct keys stored as representatives: index into k arrays of first occurrence */
        static size_t *reps = NULL;
        static size_t reps_cap = 0;
        if (reps_cap < A369467_cap + 1) {
            reps = (size_t *)realloc(reps, (A369467_cap + 1) * sizeof(size_t));
            reps_cap = A369467_cap + 1;
        }
        for (size_t m = A369467_done + 1; m <= N; ++m) {
            uint64_t d1 = A369467_deriv_odd((uint64_t)m);
            uint64_t d2 = A369467_deriv_odd(A369467_a163511((uint64_t)m));
            A369467_k1[m] = d1;
            A369467_k2[m] = d2;
            size_t found = 0;
            for (size_t j = 0; j < A369467_distinct; ++j) {
                size_t r = reps[j];
                if (A369467_k1[r] == d1 && A369467_k2[r] == d2) {
                    found = j + 1;
                    break;
                }
            }
            if (!found) {
                reps[A369467_distinct++] = m;
                found = A369467_distinct;
            }
            A369467_res[m] = (uint64_t)found;
        }
        A369467_done = N;
    }
    return (seq_t)A369467_res[N];
}
