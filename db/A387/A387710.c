/*
 * A387710  Numbers k for which A003959(k) < 2*k, where A003959 is
 *          multiplicative with a(p^e) = (p+1)^e.
 * https://oeis.org/A387710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 3653, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 94 terms through n = 9138 (2026-11-01)
 * Helpers:   factor_u64, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that A003959(k) < 2 k
 * A003959 is multiplicative with a(p^e) = (p+1)^e.
 * For each k, compute A003959(k) and compare with 2*k.
 * The function a(n) returns the n‑th such k (offset 1). */

/* Required helpers from utils */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
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

static idx_t next_k = 1;          /* next integer to test */
static seq_t *seq_vals = NULL;    /* cached sequence values */
static size_t seq_cap = 0;        /* allocated capacity of seq_vals */
static size_t seq_len = 0;        /* number of terms stored */

/* Grow seq_vals to at least 'need' elements */
static void ensure_seq_capacity(size_t need)
{
    if (need <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    if (new_cap < need)
        new_cap = need;
    seq_vals = seq_realloc(seq_vals, seq_cap, new_cap);
    seq_cap = new_cap;
}

/* Test whether a given k satisfies the condition */
static bool satisfies(idx_t k)
{
    /* Factor k */
    uint64_t primes_buf[64];
    uint64_t exps_buf[64];
    size_t fac_cnt = 0;
    factor_u64(k, primes_buf, exps_buf, &fac_cnt);

    /* Compute A003959(k) = ∏ (p+1)^e */
    seq_t prod = (seq_t)1;
    for (size_t i = 0; i < fac_cnt; ++i) {
        seq_t term = ipow_idx((seq_t)(primes_buf[i] + 1), (idx_t)exps_buf[i]);
        prod = seq_mul(prod, term);
    }

    /* Compare with 2*k */
    seq_t two_k = mul_small((seq_t)k, 2u);
    return prod < two_k;
}

/* The required function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for 0, return 0 safely */

    /* Generate terms until we have at least n of them */
    while (seq_len < n) {
        if (satisfies(next_k)) {
            ensure_seq_capacity(seq_len + 1);
            seq_vals[seq_len] = (seq_t)next_k;
            ++seq_len;
        }
        ++next_k;
    }
    return seq_vals[n - 1];
}
