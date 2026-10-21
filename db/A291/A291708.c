/*
 * A291708  Number of partitions of n into two prime parts or two nonprime
 *          parts.
 * https://oeis.org/A291708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor(n/2)} [is_prime(i) == is_prime(n-i)]
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Forward declarations for static helpers */
static void _extend_primes(idx_t limit);
static void _extend_cache(idx_t limit);

/* Global state */
static bool *isp = NULL;          /* Prime indicator array */
static idx_t  isp_cap = 0;        /* Current size of isp (max n covered) */

static seq_t *a_cache = NULL;     /* Computed a(n) values */
static idx_t  a_cap = 0;          /* Allocated size of a_cache */
static idx_t  a_max = 0;          /* Largest n with a_cache[n] computed */

/* Sieve of Eratosthenes up to `limit`.  Reallocates isp if necessary. */
static void _extend_primes(idx_t limit)
{
    if (isp_cap >= limit) return;

    bool *new_isp = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!new_isp) exit(1);

    for (idx_t i = 0; i <= limit; ++i) new_isp[i] = true;
    if (limit >= 0) new_isp[0] = false;
    if (limit >= 1) new_isp[1] = false;

    for (idx_t p = 2; p * p <= limit; ++p)
        if (new_isp[p]) {
            for (idx_t j = p * p; j <= limit; j += p)
                new_isp[j] = false;
        }

    free(isp);
    isp = new_isp;
    isp_cap = limit;
}

/* Ensure cache can hold values up to `limit` and compute any missing ones. */
static void _extend_cache(idx_t limit)
{
    if (a_cap >= limit + 1) return;

    seq_t *new_cache = (seq_t *)realloc(a_cache, (limit + 1) * sizeof(seq_t));
    if (!new_cache) exit(1);
    a_cache = new_cache;
    a_cap = limit + 1;

    for (idx_t n = a_max + 1; n <= limit; ++n) {
        seq_t cnt = 0;
        for (idx_t i = 1; i <= n / 2; ++i)
            if (isp[i] == isp[n - i]) ++cnt;
        a_cache[n] = cnt;
    }
    a_max = limit;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* Undefined for n=0, but return 0 safely */

    _extend_primes(n);
    _extend_cache(n);

    return a_cache[n];
}
