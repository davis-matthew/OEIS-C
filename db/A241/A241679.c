/*
 * A241679  2^p + 7 where p is prime.
 * https://oeis.org/A241679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^p_n + 7 where p_n is the nth prime (a(1) = 2^2+7 = 11)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   build_primes_upto, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/build_primes_upto.h"   /* provides build_primes_upto */

static uint64_t *prime_cache = NULL;   /* array of primes (type idx_t == uint64_t)      */
static size_t prime_cache_cnt = 0;     /* number of primes stored                       */
static bool prime_cache_init = false;

/* Make sure at least n primes are available in prime_cache (zero‑based index) */
static void ensure_primes(idx_t n)
{
    if (prime_cache_init && prime_cache_cnt >= n) return;

    /* Simple upper bound for the nth prime when n >= 6:
       p_n < n (log n + log log n)  ; for small n we use a fixed limit. */
    idx_t limit;
    if (n < 6) limit = 15;
    else  limit = n * 20;     /* over‑estimate – enough for n ≤ 10000  */

    if (limit < 100) limit = 100;

    /* Free old cache so we don't leak memory */
    if (prime_cache) free(prime_cache);

    /* Build new prime list up to 'limit' */
    build_primes_upto(limit, &prime_cache, &prime_cache_cnt);

    prime_cache_init = true;
}

/* Return the a(n) value: 2^(p_n) + 7 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset 1 – behaviour for n = 0 is irrelevant */

    /* Ensure we have at least n primes (zero‑based index n-1) */
    ensure_primes(n);

    /* Get the nth prime (1‑based) */
    idx_t prime = prime_cache[n-1];

    /* Compute 2^prime + 7 */
    seq_t val = pow2(prime);
    val += (seq_t)7;
    return val;
}
