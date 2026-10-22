/*
 * A303206  Number of partitions of n into two prime parts (p,q) such that
 *          |q-p| is squarefree.
 * https://oeis.org/A303206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i=1..⌊(n-1)/2⌋} is_prime(i) * is_prime(n-i) *
 *            is_squarefree(n-2*i)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A303206: number of partitions of n into two primes (p,q) with |q-p| squarefree. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* static caches for prime and square‑free flags, reallocated when a larger n is needed */
static idx_t _max_sieve = 0;
static unsigned char *_is_prime   = NULL;
static unsigned char *_is_sqfree  = NULL;

/* extend the sieves to cover all numbers ≤ n (if needed) */
static void _ensure_sieve(idx_t n)
{
    if (n <= _max_sieve) return;   /* already ready */

    /* allocate new arrays */
    unsigned char *prime  = (unsigned char*)malloc((size_t)(n + 1));
    unsigned char *sqfree = (unsigned char*)malloc((size_t)(n + 1));
    if (!prime || !sqfree) {
        /* terminate on allocation failure */
        abort();
    }

    /* initialise all entries to “true” (1) */
    memset(prime, 1, (size_t)(n + 1));
    memset(sqfree, 1, (size_t)(n + 1));

    /* zero 0 and 1 for the prime flag */
    if (n >= 0) prime[0] = 0;
    if (n >= 1) prime[1] = 0;

    /* Sieve of Eratosthenes for primes */
    for (idx_t i = 2; i <= n / i; ++i) {
        if (prime[i]) {
            for (idx_t j = i * i; j <= n; j += i) prime[j] = 0;
        }
    }

    /* Sieve for square‑free numbers: eliminate multiples of squares */
    for (idx_t i = 2; i <= n / i; ++i) {
        idx_t sq = i * i;
        for (idx_t j = sq; j <= n; j += sq) sqfree[j] = 0;
    }

    /* free old caches and store the new ones */
    free(_is_prime);
    free(_is_sqfree);
    _is_prime   = prime;
    _is_sqfree  = sqfree;
    _max_sieve  = n;
}

/* public function computing a(n) for any n >= 1 */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for non‑positive indices */

    _ensure_sieve(n);

    idx_t max_i = (n > 1) ? (n - 1) / 2 : 0;
    seq_t total = 0;

    for (idx_t i = 1; i <= max_i; ++i) {
        if (_is_prime[i] && _is_prime[n - i] && _is_sqfree[n - 2 * i]) {
            ++total;  /* increment one for each valid pair (i, n-i) */
        }
    }
    return total;
}
