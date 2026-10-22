/*
 * A364459  Number of primes in the interval [n,n+log(n)^2].
 * https://oeis.org/A364459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A364459: Number of primes in the interval [n,n+log(n)^2].
 * Formula: a(n) = #{p prime | n ≤ p ≤ n + ⌊log(n)^2⌋ }   (n ≥ 1)
 *
 * The implementation below precomputes a sieve of Eratosthenes up to a
 * dynamically chosen limit.  The sieve is expanded lazily so that the
 * function works efficiently even if the calls are in arbitrary order.
 * It uses only integer arithmetic for the final count; the log is
 * evaluated with double precision, which is safe for all n that fit in
 * the usual 64‑bit integer range (the interval length is at most a few
 * hundred for n < 10^6).
 *
 * The function returns the correct value for any n ≥ 1; for n = 0 it
 * returns 0.  All static data are thread‑unsafe (as in the original
 * problem statement); use external locks if needed.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>

static idx_t sieve_limit = 0;           /* highest value that has been sieved */
static size_t prime_count  = 0;          /* number of primes currently stored */
static uint64_t *primes     = NULL;      /* array of primes */
static bool *is_prime       = NULL;      /* sieve bitmap */

static void expand_sieve(idx_t new_limit) {
    if (new_limit <= sieve_limit)
        return;

    /* Allocate or reallocate sieve array */
    size_t size = (size_t)new_limit + 1;
    bool *new_sieve = (bool*)calloc(size, sizeof(bool));
    assert(new_sieve);
    if (sieve_limit > 0) {
        /* copy existing sieve data */
        memcpy(new_sieve, is_prime, (size_t)(sieve_limit + 1) * sizeof(bool));
        free(is_prime);
    }
    is_prime = new_sieve;

    /* Sieving for new numbers */
    for (idx_t i = 2; i * i <= new_limit; ++i) {
        if (!is_prime[i]) {
            for (idx_t j = i * i; j <= new_limit; j += i)
                is_prime[j] = true;
        }
    }

    /* Build the prime list for the new range */
    for (idx_t i = (sieve_limit >= 2 ? sieve_limit + 1 : 2); i <= new_limit; ++i) {
        if (!is_prime[i]) {
            /* grow primes array if needed */
            if (prime_count == 0 || (prime_count & (prime_count - 1)) == 0) {
                size_t new_cap = prime_count == 0 ? 8 : prime_count << 1;
                uint64_t *tmp = (uint64_t*)realloc(primes, new_cap * sizeof(uint64_t));
                assert(tmp);
                primes = tmp;
            }
            primes[prime_count++] = (uint64_t)i;
        }
    }

    sieve_limit = new_limit;
}

/* Binary search to find first prime >= key */
static size_t lower_bound(idx_t key) {
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] < (uint64_t)key)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

/* Binary search to find first prime > key */
static size_t upper_bound(idx_t key) {
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] <= (uint64_t)key)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

seq_t a(idx_t n) {
    if (n < 1)
        return 0;

    /* Interval length = floor(log(n)^2) */
    double ln = log((double)n);
    idx_t len = (idx_t)(ln * ln);

    idx_t interval_end = n + len;

    /* Ensure we have primes up to interval_end */
    expand_sieve(interval_end);

    size_t left  = lower_bound(n);
    size_t right = upper_bound(interval_end);

    return (seq_t)(right - left);
}
