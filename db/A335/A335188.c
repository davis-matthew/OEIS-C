/*
 * A335188  Total surface area of all rectangular prisms with dimensions p X q
 *          X q, where p and q are prime, n = p+q and p<q.
 * https://oeis.org/A335188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * Σ_{i=1}^{⌊(n‑1)/2⌋} (n‑i)(n+i) * c(i) * c(n‑i) where
 *            c(x)=1 if x is prime, 0 otherwise. The function uses a cache of
 *            primes up to the greatest n seen so far
 * Range:     u8 n <= 12, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Cache for primality information */
static uint8_t *primes_cache = NULL;   /* 0/1 array: 1 = prime, 0 = composite */
static idx_t   cache_limit   = 0;      /* current maximum index fully sieved */

/* Allocate and sieve up to <limit>.  If a larger cache is needed,
   the previous one is freed.  This routine is safe for both C and C++. */
static void
_ensure_sieve(idx_t limit)
{
    if (limit <= cache_limit) return;          /* already available */

    /* Allocate new cache (size limit+1, zero-initialized) */
    uint8_t *new_cache = (uint8_t *)calloc((size_t)limit + 1, sizeof(uint8_t));
    if (!new_cache) exit(EXIT_FAILURE);        /* allocation failed */

    /* 0 and 1 are not prime.  Mark all other entries tentatively as prime. */
    if (limit >= 0) new_cache[0] = 0;
    if (limit >= 1) new_cache[1] = 0;
    for (idx_t i = 2; i <= limit; ++i) new_cache[i] = 1;

    /* Sieve of Eratosthenes */
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (new_cache[p]) {
            for (idx_t q = p * p; q <= limit; q += p) {
                new_cache[q] = 0;
            }
        }
    }

    /* Replace old cache */
    free(primes_cache);
    primes_cache = new_cache;
    cache_limit   = limit;
}

/* Public function required by the specification */
seq_t a(idx_t n)
{
    /* No Goldbach partitions when n < 3, but the loop handles it automatically. */
    _ensure_sieve(n);                         /* need all primes up to n */

    seq_t sum = 0;
    idx_t lim = (n - 1) / 2;                  /* floor((n-1)/2) */

    for (idx_t i = 1; i <= lim; ++i) {
        if (!primes_cache[i] || !primes_cache[n - i]) continue;   /* not a prime pair */
        seq_t term = (seq_t)(n - i) * (seq_t)(n + i);             /* (n-i)(n+i) */
        term <<= 1;                                            /* multiply by 2 */
        sum += term;
    }
    return sum;
}
