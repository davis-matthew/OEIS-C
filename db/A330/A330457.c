/*
 * A330457  Number of multisets of nonempty multisets of nonempty sets of
 *          positive integers with total sum n.
 * https://oeis.org/A330457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = EulerTransform(EulerTransform(p_distinct)) where
 *            p_distinct(k) = number of partitions of k into distinct parts.
 *            The recurrence used is derived from a_n = (1/n) * Σ_{k=1..n} (
 *            Σ_{d|k} d * s_d ) * a_{n-k} applied twice: first for s =
 *            p_distinct, then for a = EulerTransform(s)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 26, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static storage for intermediate sequences */
static seq_t *p_distinct = NULL;   /* p_distinct[k]  : partitions into distinct parts  */
static seq_t *b1 = NULL;           /* b1[k] = Σ_{d|k} d * p_distinct[d]          */
static seq_t *s = NULL;            /* s[k]   : EulerTransform(p_distinct)         */
static seq_t *b2 = NULL;           /* b2[k]  = Σ_{d|k} d * s[d]                   */
static seq_t *a_seq = NULL;        /* a_seq[k]  : desired sequence                */
static size_t capacity = 0;        /* current allocated array size                */
static idx_t cached_n = 0;

/* Allocate or grow arrays up to (n+1) elements and recompute all terms. */
static void ensure_size(idx_t n)
{
    if (n < cached_n)
        return;                     /* already computed up to this n   */

    size_t new_size = n + 1;
    if (new_size > capacity) {
        capacity = new_size;
        size_t sz = capacity * sizeof(seq_t);
        p_distinct = (seq_t*)realloc(p_distinct, sz);
        b1          = (seq_t*)realloc(b1,          sz);
        s           = (seq_t*)realloc(s,           sz);
        b2          = (seq_t*)realloc(b2,          sz);
        a_seq       = (seq_t*)realloc(a_seq,       sz);
        if (!p_distinct || !b1 || !s || !b2 || !a_seq)
            abort();                 /* allocation failed */
    }

    /* clear arrays up to n */
    for (idx_t i = 0; i <= n; ++i) {
        p_distinct[i] = 0;
        b1[i]          = 0;
        s[i]           = 0;
        b2[i]          = 0;
        a_seq[i]       = 0;
    }

    /* 1. partitions into distinct parts: p_distinct */
    p_distinct[0] = 1;
    for (idx_t part = 1; part <= n; ++part) {
        for (idx_t sum = n; sum >= part; --sum) {
            p_distinct[sum] += p_distinct[sum - part];
        }
    }

    /* 2. first divisor sum: b1[k] = Σ_{d|k} d * p_distinct[d] */
    for (idx_t d = 1; d <= n; ++d) {
        seq_t val = (seq_t)d * p_distinct[d];
        for (idx_t m = d; m <= n; m += d) {
            b1[m] += val;
        }
    }

    /* 3. first Euler transform: s[0] = 1; s[k] = (1/k) Σ_{i=1..k} b1[i] * s[k-i] */
    s[0] = 1;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t rhs = 0;
        for (idx_t i = 1; i <= k; ++i) {
            rhs += b1[i] * s[k - i];
        }
        s[k] = rhs / (seq_t)k;          /* exact division */
    }

    /* 4. second divisor sum: b2[k] = Σ_{d|k} d * s[d] */
    for (idx_t d = 1; d <= n; ++d) {
        seq_t val = (seq_t)d * s[d];
        for (idx_t m = d; m <= n; m += d) {
            b2[m] += val;
        }
    }

    /* 5. second Euler transform: a_seq[0] = 1; a_seq[k] = (1/k) Σ_{i=1..k} b2[i] * a_seq[k-i] */
    a_seq[0] = 1;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t rhs = 0;
        for (idx_t i = 1; i <= k; ++i) {
            rhs += b2[i] * a_seq[k - i];
        }
        a_seq[k] = rhs / (seq_t)k;
    }

    cached_n = n;
}

seq_t a(idx_t n)
{
    ensure_size(n);
    return a_seq[n];
}
