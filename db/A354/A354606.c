/*
 * A354606  a(1) = 1; for n > 1, a(n) is number of terms in the first n-1
 *          terms of the sequence that have the same number of divisors as
 *          a(n-1).
 * https://oeis.org/A354606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1; for n>1, a(n)=# of k≤n-1 with τ(a(k))=τ(a(n-1)), where
 *            τ(m) is the number of divisors of m
 * Range:     u8 n <= 841, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/divisor_count_u64.h"

/* ---------- Helper: divisor count for seq_t values ---------- */
static inline unsigned divisor_count_seq(seq_t x)
{
    /* The sequence values are small; we cast to 64‑bit for divisor counting. */
    return divisor_count_u64((uint64_t)x);
}

/* ---------- Static storage for memoization ---------- */
static seq_t *a_vals = NULL;          /* array of sequence values (1‑based) */
static size_t a_cap = 0;              /* capacity of a_vals */
static size_t a_len = 0;              /* number of terms already computed */

static int *freq = NULL;              /* freq[d] = # of previous terms with τ = d */
static size_t freq_cap = 0;           /* current capacity of freq */

/* Ensure a_vals can hold at least 'need' elements. */
static void ensure_a_cap(size_t need)
{
    if (need <= a_cap) return;
    size_t new_cap = a_cap ? a_cap * 2 : 64;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    a_vals = tmp;
    a_cap = new_cap;
}

/* Ensure freq can index up to 'idx' (i.e., size > idx). */
static void ensure_freq_cap(size_t idx)
{
    if (idx + 1 <= freq_cap) return;
    size_t new_cap = freq_cap ? freq_cap * 2 : 8;
    while (new_cap <= idx) new_cap <<= 1;
    int *tmp = (int *)realloc(freq, new_cap * sizeof(int));
    if (!tmp) exit(EXIT_FAILURE);
    /* zero the new part */
    for (size_t i = freq_cap; i < new_cap; ++i) tmp[i] = 0;
    freq = tmp;
    freq_cap = new_cap;
}

/* Extend the precomputed sequence up to index n (1‑based). */
static void compute_up_to(idx_t n)
{
    if (n == 0) return;
    if ((size_t)n <= a_len) return;            /* already computed */

    /* Allocate freq array if it doesn't exist yet. */
    if (freq == NULL) {
        ensure_freq_cap(0);                     /* allocate minimal size */
    }

    /* Grow a_vals array if necessary. */
    ensure_a_cap((size_t)n);

    /* Compute missing terms. */
    for (size_t idx = a_len + 1; idx <= (size_t)n; ++idx) {
        seq_t new_val;
        if (idx == 1) {
            new_val = (seq_t)1;
        } else {
            seq_t prev_val = a_vals[idx - 2];            /* a(n-1) */
            unsigned d_prev = divisor_count_seq(prev_val);
            ensure_freq_cap(d_prev);                     /* safety: freq contains d_prev at least */
            new_val = (seq_t)freq[d_prev];
        }

        /* Store the new value. */
        a_vals[idx - 1] = new_val;
        ++a_len;

        /* Update frequency of its divisor count. */
        unsigned d_new = divisor_count_seq(new_val);
        ensure_freq_cap(d_new);
        ++freq[d_new];
    }
}

/* ---------- Public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, but harmless */
    compute_up_to(n);
    return a_vals[(size_t)n - 1];
}
