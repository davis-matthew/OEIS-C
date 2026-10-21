/*
 * A297707  a(n) = Product_{k=1..n-1} n!k, where n!k is k-tuple factorial of
 *          n.
 * https://oeis.org/A297707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(n-1) * ∏_{k=1}^{n-1} k^τ(n-k) (τ(m)=number of divisors
 *            of m)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static uint32_t *tau_cache = NULL;   /* τ(m) for 0 ≤ m ≤ current max */
static idx_t     tau_max   = 0;      /* current maximum n-1 cached */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;   /* a(1) = 1 */

    /* ------------------------------------------------------------
       Build or extend the divisor‑count cache up to n-1.
       ------------------------------------------------------------ */
    idx_t limit = n - 1;
    if (limit > tau_max) {
        idx_t new_limit = limit;
        uint32_t *tmp   = (uint32_t *)realloc(tau_cache,
                                                (new_limit + 1) * sizeof(uint32_t));
        if (!tmp) return 0;                     /* allocation failure (unlikely) */
        /* zero the newly added entries */
        if (new_limit > tau_max)
            memset(tmp + tau_max + 1,
                   0,
                   (new_limit - tau_max) * sizeof(uint32_t));
        /* increment divisor counts for the new range */
        for (idx_t i = 1; i <= new_limit; ++i) {
            idx_t start = i * ((tau_max / i) + 1);   /* first multiple > tau_max */
            for (idx_t j = start; j <= new_limit; j += i)
                ++tmp[j];
        }
        tau_cache = tmp;
        tau_max   = new_limit;
    }

    /* ------------------------------------------------------------
       Compute a(n) = n^(n-1) * ∏_{k=1}^{n-1} k^τ(n-k)
       ------------------------------------------------------------ */
    seq_t result = (seq_t)1;

    /* n^(n-1) */
    for (idx_t i = 0; i < n - 1; ++i)
        result *= (seq_t)n;

    /* ∏_{k=1}^{n-1} k^τ(n-k) */
    for (idx_t k = 1; k <= limit; ++k) {
        seq_t base = (seq_t)k;
        uint32_t exp = tau_cache[n - k];
        for (uint32_t e = 0; e < exp; ++e)
            result *= base;
    }

    return result;
}
