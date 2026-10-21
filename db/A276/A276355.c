/*
 * A276355  Sum of primes between 100*n and 100*n + 99.
 * https://oeis.org/A276355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = sum of primes p with 100*n ≤ p ≤ 100*n + 99
 * Range:     u8 none, u16 n <= 41, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>


static idx_t      _sieve_limit   = 0;      /* maximum number already sieved */
static bool      *_sieve_is_prime = NULL; /* primality flag array (0.._sieve_limit) */
static seq_t     *_sieve_pref_sum = NULL; /* prefix sums of primes (0.._sieve_limit) */

/* enlarge the sieve and prefix sum up to at least new_limit */
static void _ensure_sieve(idx_t new_limit)
{
    if (new_limit <= _sieve_limit)
        return;

    /* allocate and initialise primality array */
    bool *new_is_prime = (bool *)malloc((size_t)(new_limit + 1) * sizeof(bool));
    if (!new_is_prime)
        return;                                     /* memory exhaustion – caller will return 0 */

    for (idx_t i = 0; i <= new_limit; ++i)
        new_is_prime[i] = true;                     /* assume all prime */

    if (new_limit >= 0) new_is_prime[0] = false;
    if (new_limit >= 1) new_is_prime[1] = false;

    /* sieve of Eratosthenes */
    for (idx_t p = 2; p * p <= new_limit; ++p)
        if (new_is_prime[p])
            for (idx_t m = p * p; m <= new_limit; m += p)
                new_is_prime[m] = false;

    /* allocate and fill prefix sum array */
    seq_t *new_pref_sum = (seq_t *)malloc((size_t)(new_limit + 1) * sizeof(seq_t));
    if (!new_pref_sum) {
        free(new_is_prime);
        return;
    }
    new_pref_sum[0] = 0;
    for (idx_t i = 1; i <= new_limit; ++i) {
        new_pref_sum[i] = new_pref_sum[i - 1];
        if (new_is_prime[i])
            new_pref_sum[i] += (seq_t)i;
    }

    /* replace old tables */
    free(_sieve_is_prime);
    free(_sieve_pref_sum);
    _sieve_is_prime = new_is_prime;
    _sieve_pref_sum = new_pref_sum;
    _sieve_limit   = new_limit;
}

seq_t a(idx_t n)
{
    idx_t low  = (idx_t)100 * n;
    idx_t high = low + 99;

    _ensure_sieve(high);
    if (!_sieve_pref_sum)
        return (seq_t)0;  /* failed to build sieve */

    if (low == 0)
        return _sieve_pref_sum[high];
    else
        return _sieve_pref_sum[high] - _sieve_pref_sum[low - 1];
}
