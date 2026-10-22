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
 * Helpers:   factor_u64, mul_small, seq_mul, seq_pow, seq_realloc
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
#include "../utils/factor_u64.h"   /* factor_u64 */
#include "../utils/seq_mul.h"     /* seq_mul */
#include "../utils/seq_pow.h"     /* seq_pow */
#include "../utils/mul_small.h"   /* mul_small */
#include "../utils/seq_realloc.h" /* seq_realloc */

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
        seq_t term = seq_pow((seq_t)(primes_buf[i] + 1), (idx_t)exps_buf[i]);
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
