/*
 * A308100  Sum of the largest sides of all integer-sided triangles with prime
 *          sides and perimeter n.
 * https://oeis.org/A308100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum over all prime triples (p,q,r) with p+q+r=n, * p <= q
 *            <= r and p+q > r, of the largest side r
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes all primes up to the maximum
 * requested n using a simple sieve and then enumerates all
 * (p,q,r) satisfying the conditions.  The primes list is kept in static
 * memory and rebuilt only when a larger n is requested. */

#include <stdlib.h>   /* malloc, calloc, free */
#include <stdbool.h>  /* bool */

static uint64_t *primes = NULL;      /* array of primes up to current limit */
static size_t   primes_cnt = 0;      /* number of primes in the array     */
static idx_t    prime_limit = 0;     /* largest n for which 'primes' is valid */

/* Build or extend the primes list up to (and including) limit n.
 * Recomputes the whole list if n is larger than the current limit. */
static void ensure_primes_upto(idx_t n)
{
    if (n <= prime_limit)
        return;

    /* Allocate and initialise a boolean array for the sieve. */
    bool *is_prime = (bool *)calloc((size_t)n + 1, sizeof(bool));
    if (!is_prime)
        abort();  /* memory allocation failed */

    for (idx_t i = 2; i <= n; ++i)
        is_prime[i] = true;

    for (idx_t p = 2; p * p <= n; ++p) {
        if (is_prime[p]) {
            for (idx_t j = p * p; j <= n; j += p)
                is_prime[j] = false;
        }
    }

    /* Count primes. */
    size_t cnt = 0;
    for (idx_t i = 2; i <= n; ++i)
        if (is_prime[i]) ++cnt;

    /* Allocate and fill new primes array. */
    uint64_t *new_primes =
        (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!new_primes) {
        free(is_prime);
        abort();
    }
    size_t idx = 0;
    for (idx_t i = 2; i <= n; ++i)
        if (is_prime[i])
            new_primes[idx++] = (uint64_t)i;

    free(is_prime);

    /* Replace the old primes array. */
    free(primes);
    primes = new_primes;
    primes_cnt = cnt;
    prime_limit = n;
}

/* Binary search for a 64-bit prime in the stored primes list. */
static bool prime_lookup(uint64_t x)
{
    if (x > prime_limit || prime_limit < 2)
        return false;

    size_t lo = 0, hi = primes_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        uint64_t v = primes[mid];
        if (v == x)
            return true;
        if (v < x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return false;
}

/* Main function required by the kernel -------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Ensure we have all primes up to n. */
    ensure_primes_upto(n);

    seq_t total = 0;

    /* Iterate over primes p ≤ n/3 (since p ≤ q ≤ r). */
    for (size_t ip = 0; ip < primes_cnt; ++ip) {
        uint64_t p = primes[ip];
        if (p > n / 3)
            break;

        /* Iterate over primes q from current p upwards, while q ≤ (n‑p)/2. */
        for (size_t iq = ip; iq < primes_cnt; ++iq) {
            uint64_t q = primes[iq];
            if (q > (n - p) / 2)
                break;

            uint64_t sum_pq = p + q;
            /* Triangle inequality: p + q > r ⇒ p + q > n - p - q. */
            if (sum_pq <= n / 2)
                continue;

            uint64_t r = n - sum_pq;  /* third side */
            if (prime_lookup(r))
                total += (seq_t)r;
        }
    }

    return total;
}
