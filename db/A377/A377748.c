/*
 * A377748  a(n) = (A000217(n) - A005132(n))/2.
 * https://oeis.org/A377748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A000217(n) - A005132(n)) / 2
 * Range:     u8 n <= 34, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*        = sum_{k=1..n} max(0, - (A005132(k)))  */
/*          (Sum of the down steps in Recamán's sequence) */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* --- Static memoisation tables ---------------------------------- */
static seq_t *recam = NULL;        /* Recamán terms   : recam[i] = A005132(i) */
static seq_t *cum   = NULL;        /* Cumulative downs: cum[i]   = a(i)         */
static seq_t *seen  = NULL;        /* All values that have appeared so far   */
static size_t recam_cap = 0;       /* capacity of recam array   */
static size_t cum_cap   = 0;       /* capacity of cum   array   */
static size_t seen_cap  = 0;       /* capacity of seen array  */
static size_t seen_len  = 0;       /* number of elements in seen */
static idx_t  max_computed = 0;    /* highest n that has been filled */

/* ---------------------------------------------------------------- */
/* Resize logic ---------------------------------------------------- */
/* Ensure that the recam array can hold indices up to 'need' (inclusive). */
static void ensure_recam_capacity(size_t need) {
    if (recam_cap >= need) return;
    size_t cap = recam_cap ? recam_cap * 2 : 1;
    while (cap < need) cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(recam, cap * sizeof(seq_t));
    if (!tmp) abort();
    for (size_t i = recam_cap; i < cap; ++i) tmp[i] = 0;
    recam = tmp; recam_cap = cap;
}

/* Ensure that the cum array can hold indices up to 'need' (inclusive). */
static void ensure_cum_capacity(size_t need) {
    if (cum_cap >= need) return;
    size_t cap = cum_cap ? cum_cap * 2 : 1;
    while (cap < need) cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cum, cap * sizeof(seq_t));
    if (!tmp) abort();
    for (size_t i = cum_cap; i < cap; ++i) tmp[i] = 0;
    cum = tmp; cum_cap = cap;
}

/* Ensure that the seen array can hold at least 'need' elements. */
static void ensure_seen_capacity(size_t need) {
    if (seen_cap >= need) return;
    size_t cap = seen_cap ? seen_cap * 2 : 1;
    while (cap < need) cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seen, cap * sizeof(seq_t));
    if (!tmp) abort();
    seen = tmp; seen_cap = cap;
}

/* ---------------------------------------------------------------- */
/* Seen‑set helpers ------------------------------------------------ */
/* Linear search for existence.  N is small (≤ 10 000) so this is fine. */
static bool seen_contains(seq_t val) {
    for (size_t i = 0; i < seen_len; ++i) {
        if (seen[i] == val) return true;
    }
    return false;
}

/* Insert into seen if not already present. */
static void seen_insert(seq_t val) {
    if (seen_contains(val)) return;
    if (seen_len == seen_cap) ensure_seen_capacity(seen_len + 1);
    seen[seen_len++] = val;
}

/* ---------------------------------------------------------------- */
/* The required function ------------------------------------------ */
seq_t a(idx_t n) {
    /* On first call initialise the tables for index 0. */
    if (!recam) {
        ensure_recam_capacity(1);
        ensure_cum_capacity(1);
        ensure_seen_capacity(1);
        recam[0] = 0;
        cum[0]   = 0;
        seen[0]  = 0;
        seen_len = 1;
        max_computed = 0;
    }

    if (n <= max_computed) return cum[(size_t)n];

    /* Expand tables to reach 'n'. */
    size_t target = (size_t)n + 1;         /* indices 0..n inclusive */
    ensure_recam_capacity(target);
    ensure_cum_capacity(target);

    /* Compute missing terms. */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        seq_t prev = recam[(size_t)(k - 1)];
        seq_t next;
        if ((seq_t)k <= prev) {            /* subtraction possible */
            seq_t cand = prev - (seq_t)k;   /* result is non‑negative   */
            if (!seen_contains(cand))
                next = cand;                /* use the subtraction      */
            else
                next = prev + (seq_t)k;      /* otherwise add           */
        } else {
            next = prev + (seq_t)k;          /* subtraction would be negative */
        }
        recam[(size_t)k] = next;
        seq_t down = (next < prev) ? (prev - next) : 0;
        cum[(size_t)k] = cum[(size_t)(k - 1)] + down;
        seen_insert(next);
    }
    max_computed = n;
    return cum[(size_t)n];
}
