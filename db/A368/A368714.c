/*
 * A368714  Numbers whose maximal exponent in their prime factorization is
 *          even.
 * https://oeis.org/A368714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate the sequence by testing each integer m ≥ 1
   for the parity of its maximal prime‑factor exponent.
   The result is stored in a static cache so that a(n) can be
   returned in O(1) after the initial sieve expansion. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* static dynamic storage for the sequence values */
static seq_t *seq_values      = NULL;  /* cache of results */
static size_t seq_cap         = 0;     /* allocated capacity */
static size_t seq_count       = 0;     /* number of terms stored */

/* Smallest prime factor table for integers up to spf_limit
   (stored as uint64_ts).  The table is rebuilt when a larger limit
   is requested. */
static uint64_t *spf          = NULL;  /*   spf[i] = smallest prime dividing i (i>=2) */
static idx_t    spf_limit     = 0;     /* current upper bound of the table */

/* Ensure the SPF table covers at least [2, new_limit] */
static void ensure_spf(idx_t new_limit)
{
    if (new_limit <= spf_limit)
        return;

    /* Allocate a new table large enough to hold up to new_limit.
       calloc initializes entries to 0. */
    uint64_t *new_spf = (uint64_t *)calloc((size_t)new_limit + 1, sizeof(uint64_t));
    if (!new_spf)
        abort();

    /* Classic sieve for smallest prime factor. */
    for (idx_t i = 2; i <= new_limit; ++i) {
        if (new_spf[i] == 0) {          /* i is prime */
            new_spf[i] = i;
            for (idx_t j = i; j <= new_limit; j += i)
                if (new_spf[j] == 0)
                    new_spf[j] = i;
        }
    }

    free(spf);
    spf = new_spf;
    spf_limit = new_limit;
}

/* Grow the sequence array if it cannot hold 'needed' elements. */
static void ensure_seq_cap(size_t needed)
{
    if (seq_count < seq_cap)
        return;

    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr)
        abort();

    if (seq_values) {
        for (size_t i = 0; i < seq_count; ++i)
            new_arr[i] = seq_values[i];
        free(seq_values);
    }
    seq_values = new_arr;
    seq_cap    = new_cap;
}

/* Test whether an unsigned integer m has an even maximal exponent
   in its prime factorisation.  m is handled as a 64‑bit value
   because all terms that will be needed for n ≤ 10 000 fit
   comfortably in 64 bits. */
static bool has_even_max_exp(uint64_t m)
{
    if (m == 1)
        return true;                /* 1 is counted as valid */

    uint64_t max_exp = 0;
    uint64_t tmp = m;

    while (tmp > 1) {
        uint64_t p = spf[tmp];
        uint64_t e = 0;
        while (tmp % p == 0) {
            ++e;
            tmp /= p;
        }
        if (e > max_exp)
            max_exp = e;
    }
    return (max_exp & 1ULL) == 0;
}

/* Return the n‑th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* undefined; guard against accidental zero */

    /* Extend the cache until it contains at least n terms. */
    while (seq_count < (size_t)n) {
        idx_t old_limit = spf_limit;
        idx_t new_limit = old_limit == 0 ? 16 : old_limit << 1;  /* double the limit */

        /* Ensure the SPF table covers the new range. */
        ensure_spf(new_limit);

        /* Scan the integers in the freshly covered interval. */
        for (idx_t m = old_limit + 1; m <= new_limit; ++m) {
            if (has_even_max_exp((uint64_t)m)) {
                ensure_seq_cap(seq_count + 1);
                seq_values[seq_count++] = (seq_t)m;
            }
        }
    }

    return seq_values[(size_t)n - 1];
}
