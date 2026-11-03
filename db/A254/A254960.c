/*
 * A254960  Zeroless numbers n with digits d_1, d_2, ... d_k such that d_1^3 +
 *          ... + d_k^3 is a cube.
 * https://oeis.org/A254960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 336, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 71 terms through n = 963 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Zeroless numbers n with digits d₁,…,d_k such that
 * d₁³ + … + d_k³ is a perfect cube.
 * Computed by brute‑force enumeration and caching. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor cube root by binary search */
static inline seq_t floor_cube_root(seq_t x)
{
    seq_t lo = 0, hi = 1;
    /* raise hi until hi^3 > x */
    while (ipow_i32(hi, 3) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid3 = ipow_i32(mid, 3);
        if (mid3 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Pre‑computed cubes of 0…9 */
static const seq_t CUBES[10] = {
    (seq_t)0, (seq_t)1, (seq_t)8, (seq_t)27, (seq_t)64,
    (seq_t)125, (seq_t)216, (seq_t)343, (seq_t)512, (seq_t)729
};

/* Static cache for the sequence */
static seq_t *SEQ = NULL;
static size_t SEQ_CAP = 0;
static size_t SEQ_LEN = 0;
static seq_t CUR = (seq_t)1;        /* next number to test */

/* Ensure the cache has room for at least `need` elements */
static void ensure_capacity(size_t need)
{
    if (need <= SEQ_CAP) return;
    size_t new_cap = SEQ_CAP ? SEQ_CAP << 1 : 64;
    while (new_cap < need) new_cap <<= 1;
    SEQ = seq_realloc(SEQ, SEQ_CAP, new_cap);
    SEQ_CAP = new_cap;
}

/* Test whether a number has no zero digits and that
 * the sum of the cubes of its digits is a perfect cube. */
static bool test_candidate(seq_t n)
{
    seq_t sum = 0;
    seq_t t = n;
    while (t > 0) {
        seq_t d = t % 10;
        if (d == 0) return false;          /* contains a zero → skip */
        sum += CUBES[(int)d];
        t /= 10;
    }
    seq_t c = floor_cube_root(sum);
    return c * c * c == sum;
}

/* Return the n-th term (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* low‐index behaviour irrelevant */

    /* Return cached value if already computed */
    if ((size_t)(n - 1) < SEQ_LEN) return SEQ[n - 1];

    /* Generate terms until we reach the desired index */
    while (SEQ_LEN < n) {
        if (test_candidate(CUR)) {
            ensure_capacity(SEQ_LEN + 1);
            SEQ[SEQ_LEN++] = CUR;
        }
        CUR++;
    }
    return SEQ[n - 1];
}
