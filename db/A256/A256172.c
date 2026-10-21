/*
 * A256172  Primes of the form 6*p + 1 with p prime that are also of the form
 *          x^2 + 27*y^2 and congruent to 7 mod 24.
 * https://oeis.org/A256172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 21, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   floor_sqrt, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth term of OEIS A256172.
   Formula: a(n) = the nth prime p such that
               p ≡ 7 (mod 24),
               p = 6*q + 1 with q prime,
               and p can be expressed as x² + 27·y² for some integers x,y.
   The implementation searches candidates in increasing order,
   caching found terms to serve any subsequent calls in O(1). */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/floor_sqrt.h"

/* Test whether a prime of the form x² + 27·y² exists. */
static bool has_representation(seq_t n)
{
    /* Maximum y such that 27*y² ≤ n. */
    seq_t div = n / (seq_t)27;
    idx_t max_y = (idx_t)floor_sqrt(div);   /* y fits into an idx_t. */
    for (idx_t y = 0; y <= max_y; ++y)
    {
        seq_t rem = n - (seq_t)27 * (seq_t)y * (seq_t)y;
        seq_t x   = floor_sqrt(rem);
        if (x * x == rem)
            return true;                  /* representation found. */
    }
    return false;
}

seq_t a(idx_t n)
{
    /* ---- Static cache of found terms ---- */
    static seq_t *terms = NULL;          /* array of terms */
    static size_t len   = 0;             /* number of terms stored */
    static size_t cap   = 0;             /* allocated capacity */
    static idx_t  cand  = 31;            /* next candidate of form 7 mod 24 */

    /* Generate terms until we have at least n of them. */
    while (len < n)
    {
        seq_t cand_seq = (seq_t)cand;

        /* Candidate must be prime. */
        if (is_prime((idx_t)cand))
        {
            /* Check that (cand - 1)/6 is prime. */
            idx_t p = (cand - 1) / 6;
            if (is_prime(p))
            {
                /* Check the quadratic‑form representation. */
                if (has_representation(cand_seq))
                {
                    /* Grow cache if necessary. */
                    if (len + 1 > cap)
                    {
                        size_t new_cap = cap ? cap * 2 : 16;
                        seq_t *new_arr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                        if (!new_arr) /* allocation failed */
                            return 0;  /* failure value (out of scope for tests) */
                        terms = new_arr;
                        cap   = new_cap;
                    }
                    terms[len++] = cand_seq;
                }
            }
        }

        /* Next number of the form 7 mod 24. */
        cand += 24;
    }

    /* Return the requested term. */
    return terms[n - 1];
}
