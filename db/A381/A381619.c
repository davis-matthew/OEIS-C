/*
 * A381619  Sorted list of sums of 3 prices in minor currency units for a
 *          currency that has a 2-decimal minor unit, such that the riddle
 *          "sum of prices equals product of prices" has a solution, with
 *          prices expressed as floating point numbers with 2 decimals.
 * https://oeis.org/A381619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 622, u64 n <= 622, u128 n <= 622,
 *            big any n
 * Helpers:   cmp_terms
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function enumerates all triples (x,y,z) of natural numbers
   (in cents) satisfying 10000*(x+y+z)=x*y*z and returns the
   n‑th smallest sum s=x+y+z (indexed from 1).  It computes the whole
   collection once and caches the sorted sums for subsequent calls. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/cmp_terms.h"

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *terms = NULL;
    static size_t term_len = 0;
    static size_t term_cap = 0;

    /* Lazy initialisation: compute once */
    if (!terms)
    {
        term_cap = 1024;
        terms = (seq_t *)malloc(term_cap * sizeof(seq_t));
        if (!terms) return 0;   /* allocation failure */

        /* Search bounds for x,y */
        const uint64_t X_MAX = 20000ULL;   /* generous upper bound */

        for (uint64_t x = 1; x <= X_MAX; ++x)
        {
            /* y must satisfy x*y - 10000 > 0  →  y > 10000/x */
            uint64_t min_y = (10000ULL / x) + 1U;
            if (min_y < x) min_y = x;

            for (uint64_t y = min_y; ; ++y)
            {
                uint64_t denom = x * y - 10000ULL;
                if (denom == 0) continue;      /* safety, should not happen */

                uint64_t num = 10000ULL * (x + y);

                /* floor of z */
                uint64_t z_floor = num / denom;

                /* If z_floor < y, increasing y will only decrease z */
                if (z_floor < y) break;

                if (num % denom == 0)
                {
                    uint64_t z = z_floor;   /* exact integer solution */
                    /* ensure ordering: x <= y <= z */
                    if (z < y) continue;

                    /* store the sum (x+y+z) */
                    uint64_t sum = x + y + z;
                    if (term_len == term_cap)
                    {
                        term_cap *= 2;
                        seq_t *tmp = (seq_t *)realloc(terms,
                                                      term_cap * sizeof(seq_t));
                        if (!tmp)
                        {
                            free(terms);
                            terms = NULL;
                            return 0;
                        }
                        terms = tmp;
                    }
                    terms[term_len++] = (seq_t)sum;
                }
            }
        }

        /* Sort the list of sums */
        qsort(terms, term_len, sizeof(seq_t), cmp_terms);
    }

    if (n == 0) return 0;
    if ((size_t)(n - 1) >= term_len) return 0;
    return terms[(size_t)(n - 1)];
}
