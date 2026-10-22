/*
 * A351535  Odd numbers k that are not multiples of 3 and for which sigma(k)
 *          is congruent to 2 modulo 4.
 * https://oeis.org/A351535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 3765, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A351535: odd numbers not divisible by 3 whose sum of divisors
   σ(k) ≡ 2 (mod 4).  The function builds a memoised list of these
   numbers and returns the n‑th term for any 1‑based index n. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;   /* cached terms */
    static size_t  len   = 0;      /* number of cached terms */
    static size_t  cap   = 0;      /* capacity of the cache */
    static idx_t  next   = 5;      /* next odd candidate (idx_t for sigma_of) */

    /* Return cached value if available. */
    if (len >= n)
        return terms[n - 1];

    /* Extend the cache until we have the n‑th term. */
    while (len < n)
    {
        idx_t k = next;     /* current candidate as idx_t */
        next += 2;          /* next odd number */

        /* Skip multiples of 3. */
        if ((k % 3) == 0)
            continue;

        /* Compute σ(k) and check the congruence condition. */
        seq_t sigma = sigma_of(k);     /* sigma_of accepts idx_t */
        if ((sigma % 4) != 2)
            continue;

        /* Grow the cache if necessary. */
        if (len == cap)
        {
            size_t new_cap = cap ? cap << 1 : 8;
            seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof *terms);
            if (!tmp)
                return 0; /* allocation failure – return 0 as sentinel */
            terms = tmp;
            cap   = new_cap;
        }

        /* Store the found term (cast idx_t to seq_t). */
        terms[len++] = (seq_t)k;
    }

    return terms[n - 1];
}
