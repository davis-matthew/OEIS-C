/*
 * A394789  Number of positive integers <= n with exactly two odd divisors.
 * https://oeis.org/A394789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #{ m ≤ n | m = 2^k * p, with p an odd prime }
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Core idea: an integer has exactly two odd divisors iff its odd part is a prime.
   We pre‑compute primes up to the largest n requested and build a cumulative
   count array lazily so that every n can be answered in O(1) after the first
   expansion. */
#include <stdlib.h>
#include <stdbool.h>

static idx_t  cache_cap  = 0;   /* largest n cached (1‑based) */
static seq_t *cache      = NULL;/* cache[n] = a(n), 0 ≤ n ≤ cache_cap */

static size_t prime_cap  = 0;   /* largest n for which primes are known */
static bool  *prime      = NULL;/* prime[i] is true iff i is prime, 0 ≤ i ≤ prime_cap */

/* Build a simple sieve of Eratosthenes up to n, storing results in prime[].
   The whole array is recomputed each time n > prime_cap – this is inexpensive
   for n in the thousands. */
static void
sieve_primes_upto(idx_t n)
{
    size_t new_cap = (size_t)(n + 1);
    bool *new_prime = (bool *)realloc(prime, new_cap);
    if (!new_prime) return;                /* allocation failure: leave prime NULL */
    prime      = new_prime;
    prime_cap  = n;

    for (size_t i = 0; i <= prime_cap; ++i) prime[i] = true;
    if (prime_cap >= 0) prime[0] = false;
    if (prime_cap >= 1) prime[1] = false;

    for (idx_t p = 2; p * p <= n; ++p) {
        if (!prime[p]) continue;
        for (idx_t q = p * p; q <= n; q += p) prime[q] = false;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* safety for internal use */

    /* Grow prime table if needed. */
    if ((size_t)n > prime_cap) sieve_primes_upto(n);

    /* Expand cache if required. */
    if (n > cache_cap) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {                      /* allocation failure */
            return 0;
        }
        cache = tmp;

        /* Ensure cache[0] = 0 for the running sum. */
        cache[0] = 0;

        /* Compute a(k) for all new indices. */
        idx_t start = cache_cap + 1;
        if (start == 0) start = 1;        /* only when cache_cap was 0 */

        for (idx_t m = start; m <= n; ++m) {
            idx_t odd = m;
            while ((odd & 1) == 0) odd >>= 1;      /* strip factors of 2 */
            seq_t add = (odd > 1 && prime[odd]) ? 1 : 0;
            cache[m] = cache[m - 1] + add;
        }
        cache_cap = n;
    }

    return cache[n];
}
