/*
 * A256642  a(n) is the smallest number k such that the digital product of
 *          sigma(k) = n in base 10, or 0 if no such number k exists.
 * https://oeis.org/A256642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) = smallest k such that the digital product of sigma(k)
 *            equals n, or 0 if none exists
 * Range:     u8 n <= 14, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/digit_product.h"

#define LIMIT 2000000
#define ANS_LIMIT 5000000

/* Global tables */
static seq_t *sigma = NULL;        /* sigma[k] = sum of divisors of k          */
static seq_t *ans   = NULL;        /* ans[n] = smallest k with product n or 0 */

static void compute_all(void)
{
    /* Allocate and compute sigma only once */
    if (sigma != NULL) return;

    /* 1. Compute sigma for 1..LIMIT by a divisor sieve */
    sigma = (seq_t*)calloc((size_t)LIMIT + 1, sizeof(seq_t));
    if (!sigma) return;  /* fall back to 0 on allocation failure */

    for (idx_t i = 1; i <= LIMIT; ++i) {
        for (idx_t j = i; j <= LIMIT; j += i) {
            sigma[j] += (seq_t)i;
        }
    }

    /* 2. Allocate answer table */
    ans = (seq_t*)calloc((size_t)ANS_LIMIT, sizeof(seq_t));
    if (!ans) return;

    /* 3. Scan k and fill ans with the minimal k for each product */
    for (idx_t k = 1; k <= LIMIT; ++k) {
        seq_t prod = digit_product(sigma[k]);          /* product of decimal digits of sigma(k) */
        if (prod < (seq_t)ANS_LIMIT && ans[(size_t)prod] == 0) {
            ans[(size_t)prod] = k;                    /* first (smallest) k found for this product */
        }
    }
}

seq_t a(idx_t n)
{
    compute_all();                          /* ensure tables are ready */

    if (n < (idx_t)ANS_LIMIT) {
        return ans[(size_t)n];              /* 0 if none found */
    } else {
        return 0;                           /* out‑of‑range n – return 0 as unspecified */
    }
}
