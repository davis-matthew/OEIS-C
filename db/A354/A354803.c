/*
 * A354803  a(1) = 1; for n > 1, a(n) is the smallest positive number that is
 *          coprime to a(n-1) and the product a(n) * a(n-1) is distinct from
 *          all previous products a(i) * a(i-1), i=2..n-1.
 * https://oeis.org/A354803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   gcd, prod_used
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; for n>1, a(n) = the smallest positive integer
   coprime to a(n‑1) such that the product a(n)*a(n‑1) has not occurred
   before among a(i)*a(i‑1) for 2 ≤ i < n. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/prod_used.h"

/* static cache of computed terms and products (1‑based terms indexed
   from 0 in the array). */
static seq_t *terms    = NULL; /* a(1) stored at terms[0] */
static size_t terms_cap = 0;
static size_t terms_cnt = 0;   /* number of terms stored */

static seq_t *prod     = NULL; /* a(2)*a(1) at prod[0] */
static size_t prod_cap  = 0;
static size_t prod_cnt  = 0;   /* number of products stored */

/* Grow an array of seq_t elements.  Handles C++ by casting the void*
   result. */
static void grow_seq(seq_t **arr, size_t *cap, size_t needed, size_t elem_size)
{
    if (*cap == 0) {
        *cap = 4;
        *arr = (seq_t*)malloc(*cap * elem_size);
    } else {
        while (*cap < needed) {
            *cap <<= 1;
        }
        *arr = (seq_t*)realloc(*arr, *cap * elem_size);
    }
}

/* Compute the nth term (1‑based).  The function is memoised and works
   for any order of calls. */
seq_t a(idx_t n)
{
    if (n == 0) { /* safety for potential 0 indices */
        return (seq_t)0;
    }

    /* Initialise on first call. */
    if (terms == NULL) {
        grow_seq(&terms, &terms_cap, 1, sizeof(seq_t));
        terms[0] = (seq_t)1;   /* a(1) = 1 */
        terms_cnt = 1;

        /* No products yet. */
        prod_cnt = 0;
    }

    /* Return cached value if already computed. */
    if ((size_t)n <= terms_cnt) {
        return terms[(size_t)n - 1];
    }

    /* Compute terms up to index n. */
    for (size_t idx = terms_cnt + 1; idx <= (size_t)n; ++idx) {
        seq_t prev = terms[terms_cnt - 1]; /* a(idx-1) */
        seq_t cand = (seq_t)1;

        /* Search for the smallest admissible candidate. */
        for (;;) {
            if (gcd(prev, cand) == (seq_t)1) {
                seq_t prod_candidate = prev * cand;
                if (!prod_used(prod_candidate, prod_cnt, prod)) {
                    /* Store the new term. */
                    grow_seq(&terms, &terms_cap, terms_cnt + 1, sizeof(seq_t));
                    terms[terms_cnt] = cand;
                    terms_cnt++;

                    /* Store the new product. */
                    grow_seq(&prod, &prod_cap, prod_cnt + 1, sizeof(seq_t));
                    prod[prod_cnt] = prod_candidate;
                    prod_cnt++;
                    break;
                }
            }
            ++cand;
        }
    }

    return terms[(size_t)n - 1];
}
