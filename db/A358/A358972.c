/*
 * A358972  a(n) = ((...((n!^(n-1)!)^(n-2)!)^...)^2!)^1!.
 * https://oeis.org/A358972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!^A000178(n-1) (product of factorials 1!*2!*...*(n-1)!)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: left‑nested exponentiation: */
/* ((…((n!^((n-1)! )^((n-2)! )…)^1! ) */

/* Include necessary utilities */
#include <stdlib.h>
#include "../utils/big_pow.h"

/* Cache for factorials */
static seq_t *fact_cache = NULL;
static size_t fact_cache_size = 0;

/* Ensure factorial cache large enough for indices up to n */
static void ensure_factorial_cache(idx_t n)
{
    if (fact_cache_size < (size_t)n + 1) {
        free(fact_cache);
        fact_cache = (seq_t *)malloc(((size_t)n + 1) * sizeof(seq_t));
        fact_cache_size = (size_t)n + 1;
    }
}

/* Compute factorial(i) iteratively and store in cache */
static void compute_factorials(idx_t n)
{
    seq_t f = 1;
    fact_cache[0] = 1;
    for (idx_t i = 1; i <= n; ++i) {
        f *= (seq_t)i;
        fact_cache[i] = f;
    }
}

/* ----- Main function required by the task ----- */
seq_t a(idx_t n)
{
    /* Offset 1: a(1) = 1 */
    if (n < 1) {
        return 0;    /* undefined for n < 1, but never used */
    }
    if (n == 1) {
        return 1;
    }

    /* Prepare factorial cache for 0..n */
    ensure_factorial_cache(n);
    compute_factorials(n);

    /* Compute left‑nested power */
    seq_t result = big_pow(fact_cache[n], fact_cache[n - 1]);

    /* Remaining exponents from (n-2)! down to 1! */
    for (idx_t k = n - 2; k >= 1; --k) {
        result = big_pow(result, fact_cache[k]);
        if (k == 1) {
            break;   /* avoid unsigned wrap‑around */
        }
    }

    return result;
}
