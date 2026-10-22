/*
 * A393549  a(n) is the number of n-digit numbers in which at least one digit
 *          occurs more than once.
 * https://oeis.org/A393549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 9*(10^(n-1) - 9!/(10 - n)!) for n <= 10, a(n) =
 *            9*10^(n-1) for n > 10
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                /* size_t */
#include "../utils/seq_alloc_new_array.h"  /* seq_alloc_new_array */

/* Cached powers of 10: pow_cache[i] = 10^i.  Only grows, never freed. */
static seq_t *pow_cache = NULL;
static size_t pow_cap   = 0;
static idx_t  pow_max   = 0;

/* Return 10^exp as seq_t, using an internal cache. */
static seq_t pow10_cached(idx_t exp)
{
    /* Initialize cache if needed. */
    if (!pow_cache) {
        pow_cap   = 1;
        pow_cache = seq_alloc_new_array(pow_cap);
        pow_cache[0] = (seq_t)1;   /* 10^0 = 1 */
        pow_max   = 0;
    }

    /* Enlarge the array to accommodate 'exp'. */
    if (exp >= pow_cap) {
        size_t new_cap = pow_cap;
        while (new_cap <= exp) new_cap <<= 1;   /* double until large enough */
        seq_t *new_cache = seq_alloc_new_array(new_cap);
        for (size_t i = 0; i < pow_cap; ++i)
            new_cache[i] = pow_cache[i];
        pow_cache = new_cache;
        pow_cap   = new_cap;
    }

    /* Compute any missing powers. */
    for (idx_t i = pow_max + 1; i <= exp; ++i)
        pow_cache[i] = pow_cache[i - 1] * (seq_t)10;

    pow_max = exp;
    return pow_cache[exp];
}

seq_t a(idx_t n)
{
    if (n < 1)          /* undefined for offset < 1, return 0 */
        return (seq_t)0;

    seq_t pow10 = pow10_cached(n - 1);   /* 10^(n-1) */
    seq_t total = (seq_t)9 * pow10;      /* all n‑digit numbers */

    if (n <= 10) {
        /* Number of distinct‑digit n‑digit numbers:
           9 * 9 * 8 * ... * (10 - n + 1) */
        seq_t distinct = (seq_t)9;
        for (idx_t k = 1; k < n; ++k)
            distinct *= (seq_t)(10 - k);
        return total - distinct;          /* at least one repeat */
    }

    /* For n > 10 all numbers contain a repeated digit. */
    return total;
}
