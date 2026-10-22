/*
 * A371636  For any number k >= 0, let T_k be the triangle with values in {-1,
 *          0, +1} whose base corresponds to the balanced ternary expansion of
 *          k (without leading zeros) and other values, say t above u and v,
 *          satisfy t+u+v = 0 mod 3; this sequence lists the numbers k such
 *          that T_k has 3-fold rotational sym...
 * https://oeis.org/A371636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   balanced_ternary, is_symmetric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the nth integer k for which the triangle T_k built from the
 * balanced ternary expansion of k has 3‑fold rotational symmetry.
 * This implementation searches k in increasing order, building the triangle
 * for each k and testing rotational symmetry.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/balanced_ternary.h"
#include "../utils/is_symmetric.h"

/* Forward declarations of utility functions that may or may not be available */
#ifndef _COUNT
#define _COUNT
#endif

/* Function prototypes for static helpers */
static bool is_symmetric(uint64_t k);
static void extend_terms();

/* Memoization storage */
static seq_t *terms = NULL;          /* array of found terms */
static size_t terms_len = 0;         /* number of terms stored */
static size_t terms_cap = 0;         /* allocated capacity */
static uint64_t next_k = 0;          /* next k to test */

/* Ensure enough capacity for stored terms */
static void extend_terms()
{
    if (terms_len >= terms_cap)
    {
        size_t new_cap = (terms_cap == 0) ? 64 : terms_cap * 2;
        seq_t *new_terms = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_terms) return; /* allocation failure, give up */
        for (size_t i = 0; i < terms_len; ++i)
            new_terms[i] = terms[i];
        free(terms);
        terms = new_terms;
        terms_cap = new_cap;
    }
}

/* Main sequence function as required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n<1, but return 0 */

    /* Ensure zero‑based index: a(1) is first term. */
    size_t idx = (size_t)(n - 1);

    /* Generate terms until we have at least idx+1 terms. */
    while (terms_len <= idx)
    {
        if (is_symmetric(next_k))
        {
            extend_terms();
            terms[terms_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return terms[idx];
}
