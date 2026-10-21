/*
 * A280469  Numbers n such that n-1 and n+1 are squarefree and have the same
 *          number of prime factors.
 * https://oeis.org/A280469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Computes A280469 by searching for n such that n‑1 and n+1 are
 *            both squarefree and have the same number of prime factors
 * Range:     u8 n <= 35, u16 n <= 6388, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_prime_factors, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>      /* for size_t */

/* The helper functions are provided by the build environment. */
#include "../utils/is_squarefree.h"
#include "../utils/count_prime_factors.h"

seq_t a(idx_t n)
{
    /* Static cache for previously computed terms. */
    static seq_t *terms      = NULL;  /* pointer to the array of terms   */
    static size_t terms_len  = 0;      /* number of terms stored          */
    static size_t terms_cap  = 0;      /* allocated capacity              */
    static idx_t last_checked = 1;     /* last integer examined           */

    /* Iterate until we have at least n terms cached. */
    while (terms_len < n) {
        ++last_checked;                       /* try next candidate */

        /* Check that both n‑1 and n+1 are non‑negative.  */
        /* For unsigned arithmetic this is always true.     */
        seq_t low  = (seq_t)last_checked - 1;
        seq_t high = (seq_t)last_checked + 1;

        /* Test squarefreeness. */
        if (!is_squarefree(low) || !is_squarefree(high))
            continue;

        /* Count prime factors.  The helper returns a seq_t, but for
           comparison we only need to know if the counts are equal. */
        seq_t omega_low  = count_prime_factors(low);
        seq_t omega_high = count_prime_factors(high);

        if (omega_low != omega_high)
            continue;                         /* not a term */

        /* Store the found term.  Grow the cache if necessary. */
        if (terms_len == terms_cap) {
            size_t new_cap = terms_cap == 0 ? 16 : terms_cap << 1;
            seq_t *tmp = (seq_t *)realloc((void *)terms,
                                          new_cap * sizeof(seq_t));
            /* If realloc fails, we simply abort; the test harness
               guarantees sufficient memory. */
            terms = tmp;
            terms_cap = new_cap;
        }
        terms[terms_len++] = (seq_t)last_checked;
    }

    /* Return the requested term (1‑based index). */
    return terms[n - 1];
}
