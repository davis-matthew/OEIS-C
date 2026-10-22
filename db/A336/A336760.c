/*
 * A336760  a(0) = 0; for n > 0, a(n) = a(n-1) - tau(n) if nonnegative and not
 *          already in the sequence, otherwise a(n) = a(n-1) + tau(n), where
 *          tau(n) is the number of divisors of n.
 * https://oeis.org/A336760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Recurrence:  a(0) = 0;
 * For n > 0, let tau(n) be the number of divisors of n.
 * a(n) = a(n‑1) − tau(n)  if a(n‑1) ≥ tau(n) and that value has not appeared before,
 *        otherwise a(n) = a(n‑1) + tau(n).
 * This function computes a(n) for any n ≥ 0, memoising intermediate results
 * so that calls in arbitrary order are fast.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Divisor‑count cache ----------------------------------------------------- */
static seq_t *divs = NULL;      /* divisor counts up to the current limit     */
static idx_t   max_div = 0;     /* largest index for which divs has been built */

/* Build (or rebuild) the divisor‑count table up to 'limit'.              */
static void ensure_divisors(idx_t limit)
{
    if (limit <= max_div) return;                 /* already built */

    /* Allocate or resize */
    divs = (seq_t *)realloc(divs, (limit + 1) * sizeof(seq_t));
    /* Zero all entries – we will recompute them from scratch   */
    memset(divs, 0, (limit + 1) * sizeof(seq_t));

    /* Classic sieve: for each i, add 1 to every multiple j of i */
    for (idx_t i = 1; i <= limit; ++i) {
        for (idx_t j = i; j <= limit; j += i) {
            divs[j] += 1;
        }
    }
    max_div = limit;
}

/* Linear scan to test whether 'val' is already in the sequence.          */
static inline bool in_sequence(seq_t *arr, size_t len, seq_t val)
{
    for (size_t i = 0; i < len; ++i) {
        if (arr[i] == val) return true;
    }
    return false;
}

/* Sequence cache ---------------------------------------------------------- */
static seq_t *seq   = NULL;    /* a(0), a(1), …                         */
static size_t seq_cap = 0;     /* allocated capacity                     */
static size_t seq_len = 0;     /* number of computed terms               */

/* ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Initialise the sequence cache on first use. */
    if (seq_len == 0) {
        seq_cap = 64;
        seq     = (seq_t *)calloc(seq_cap, sizeof(seq_t));
        seq[0]   = (seq_t)0;          /* a(0) = 0 */
        seq_len  = 1;
    }

    /* Fast return for already computed indices. */
    if (n < (idx_t)seq_len) {
        return seq[n];
    }

    /* Ensure we have divisor counts up to n. */
    ensure_divisors((idx_t)n);

    /* Grow the sequence cache if necessary. */
    if (seq_len <= (size_t)n) {
        size_t new_cap = seq_cap;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = (idx_t)seq_len; i <= n; ++i) {
        seq_t tau  = divs[i];           /* tau(i) */
        seq_t prev = seq[i - 1];

        seq_t cand = prev - tau;        /* candidate if subtracting */
        bool use_decr = false;

        if (prev >= tau) {
            use_decr = !in_sequence(seq, seq_len, cand);
        }

        seq[i] = use_decr ? cand : prev + tau;
        ++seq_len;
    }

    return seq[n];
}
