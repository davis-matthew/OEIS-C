/*
 * A300020  For any k, the cumulative sum a(1) + a(2) + a(3) + ... + a(k)
 *          shows at least a digit 6.
 * https://oeis.org/A300020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 883, u32 n <= 9609, u64 n <= 9609,
 *            u128 n <= 9609, big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A300020:  Permutation of ℕ where the partial sums
 * always contain at least one decimal digit 6.
 * a(1) = 6, a(n) is the smallest unused positive integer that,
 * when added to the running sum, yields a sum containing digit 6.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/digit_counts.h"

/*  Static state to support memoised construction.  */
static seq_t *terms      = NULL;   /* array of terms */
static size_t terms_cap  = 0;      /* current capacity of terms array   */
static size_t terms_len  = 0;      /* number of terms computed so far   */

static unsigned char *used   = NULL;   /* bitmap of used integers */
static size_t used_cap    = 0;         /* current capacity of used array  */

static seq_t cum_sum = (seq_t)0;   /* current cumulative sum */

/* Ensure that the used array can index at least up to `need` (inclusive). */
static void
ensure_used_capacity(idx_t need)
{
    if (need < used_cap)
        return;                       /* already big enough */

    size_t new_cap = used_cap ? used_cap * 2 : 32;
    while (new_cap <= (size_t)need)
        new_cap <<= 1;

    unsigned char *new_used = (unsigned char *)realloc(used, new_cap);
    for (size_t i = used_cap; i < new_cap; ++i)
        new_used[i] = 0;

    used = new_used;
    used_cap = new_cap;
}

/* Ensure that the terms array can hold at least `need` terms. */
static void
ensure_terms_capacity(size_t need)
{
    if (need < terms_cap)
        return;

    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    while (new_cap <= need)
        new_cap <<= 1;

    seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms = new_terms;
    terms_cap = new_cap;
}

/* Check whether a seq_t value contains a decimal digit 6.  */
static bool
contains_digit6(seq_t x)
{
    unsigned char cnt[10];
    digit_counts(x, cnt);
    return cnt[6] > 0;
}

seq_t a(idx_t n)
{
    /*  n is 1‑based index.  The sequence is not defined for n < 1.  */
    if (n == 0)
        return (seq_t)0;

    /*  Grow internal state if we need to compute higher terms.  */
    while (terms_len < (size_t)n)
    {
        /*  Find smallest unused integer that keeps the running sum
         *  containing a digit 6.  */
        idx_t candidate = 1;
        for (;; ++candidate)
        {
            ensure_used_capacity(candidate);

            if (used[candidate])
                continue;   /* already used */

            seq_t potential = cum_sum + (seq_t)candidate;
            if (contains_digit6(potential))
            {
                /*  Accept this candidate.  */
                ensure_terms_capacity(terms_len + 1);
                terms[terms_len++] = (seq_t)candidate;
                used[candidate] = 1;
                cum_sum = potential;
                break;
            }
            /*  Otherwise, keep looking.  */
        }
    }

    return terms[(size_t)n - 1];
}
