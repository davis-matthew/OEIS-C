/*
 * A322437  Number of unordered pairs of factorizations of n into factors > 1
 *          where no factor of one divides any factor of the other.
 * https://oeis.org/A322437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7559, u16 n <= 7949, u32 n <= 7949, u64 n <= 7949,
 *            u128 n <= 7949, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) by enumerating all unordered factorizations of n
   into factors > 1 and counting unordered pairs of factorizations
   that are mutually non‑divisible. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Internal structure for a factorization */
typedef struct {
    idx_t *factors;
    size_t len;
} FactorList;

/* Recursive helper to generate all unordered factorizations of n */
static void
generate_factorizations(idx_t n, idx_t min_factor,
                        idx_t *curr, size_t curr_len,
                        FactorList **out, size_t *count, size_t *cap)
{
    if (n == 1) {
        /* Store a copy of current factorization */
        FactorList fl;
        fl.len  = curr_len;
        if (curr_len) {
            fl.factors = (idx_t *)malloc(curr_len * sizeof(idx_t));
            memcpy(fl.factors, curr, curr_len * sizeof(idx_t));
        } else {
            fl.factors = NULL;  /* should not happen for n>1 */
        }

        if (*count == *cap) {
            *cap = (*cap == 0) ? 4 : (*cap << 1);
            *out = (FactorList *)realloc(*out, *cap * sizeof(FactorList));
        }
        (*out)[*count] = fl;
        (*count)++;
        return;
    }

    for (idx_t d = min_factor; d <= n; ++d) {
        if (n % d == 0) {
            curr[curr_len] = d;
            generate_factorizations(n / d, d, curr, curr_len + 1, out, count, cap);
        }
    }
}

/* Check whether two factorizations have no factor dividing any factor of the other */
static bool
compatible(const FactorList *a, const FactorList *b)
{
    for (size_t i = 0; i < a->len; ++i) {
        idx_t x = a->factors[i];
        for (size_t j = 0; j < b->len; ++j) {
            idx_t y = b->factors[j];
            if ((x % y == 0) || (y % x == 0))
                return false;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;   /* a(1)=0 by definition */

    /* Allocate initial workspace for factor generation */
    FactorList *fact = NULL;
    size_t fact_count = 0, fact_cap = 0;
    idx_t curr_factors[32];   /* depth never exceeds about 16 */
    generate_factorizations(n, 2, curr_factors, 0, &fact, &fact_count, &fact_cap);

    /* Count compatible pairs */
    size_t pairs = 0;
    for (size_t i = 0; i < fact_count; ++i) {
        for (size_t j = i + 1; j < fact_count; ++j) {
            if (compatible(&fact[i], &fact[j]))
                pairs++;
        }
    }

    /* Free allocated memory */
    for (size_t i = 0; i < fact_count; ++i)
        free(fact[i].factors);
    free(fact);

    return (seq_t)pairs;
}
