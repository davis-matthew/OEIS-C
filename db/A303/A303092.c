/*
 * A303092  Balanced primes of order one ending in 1.
 * https://oeis.org/A303092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th prime ending with the digit 1 that is a balanced
 *            prime of order one (i.e., the gaps to its nearest primes on both
 *            sides are equal)
 * Range:     u8 n <= 1, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_prime.h"

/* -------------------------------------------------------------------------- */
/*  Dynamic array holding the balanced primes ending in 1                     */
/* -------------------------------------------------------------------------- */
static seq_t* balanced      = NULL;
static size_t balanced_len   = 0;
static size_t balanced_cap   = 0;

/* -------------------------------------------------------------------------- */
/*  State of the prime generator                                              */
/* -------------------------------------------------------------------------- */
static idx_t prev_prime      = 2;    /* previous known prime             */
static idx_t curr_prime      = 3;    /* current known prime              */
static idx_t next_candidate  = 5;    /* next odd number to test for prime */
static int   initialized     = 0;    /* 0 → first call                    */

/* -------------------------------------------------------------------------- */
/*  Grow the dynamic array when needed                                         */
/* -------------------------------------------------------------------------- */
static void expand_balanced(size_t new_cap)
{
    if (new_cap > balanced_cap) {
        balanced = (seq_t *)realloc(balanced, new_cap * sizeof(seq_t));
        balanced_cap = new_cap;
    }
}

/* -------------------------------------------------------------------------- */
/*  Generate primes until we have at least n balanced primes                   */
/* -------------------------------------------------------------------------- */
static void generate_until(idx_t n)
{
    while (balanced_len < n) {
        /* Find next prime after curr_prime */
        while (!is_prime(next_candidate))
            next_candidate += 2;
        idx_t next_prime = next_candidate;

        /* Is curr_prime a balanced prime ending with 1 ? */
        if ((curr_prime % 10 == 1) &&
            (curr_prime - prev_prime == next_prime - curr_prime)) {
            expand_balanced(balanced_len + 1);
            balanced[balanced_len++] = (seq_t)curr_prime;
        }

        /* Advance the window of consecutive primes */
        prev_prime      = curr_prime;
        curr_prime      = next_prime;
        next_candidate += 2;          /* candidate stays odd           */
    }
}

/* -------------------------------------------------------------------------- */
/*  Public function matching the requested signature                           */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!initialized) {
        /* All static state already set to its initial values                */
        initialized = 1;
    }
    if (n == 0)
        return (seq_t)0;              /* undefined for n==0, safety net   */

    generate_until(n);
    return balanced[n - 1];
}
