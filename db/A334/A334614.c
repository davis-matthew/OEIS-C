/*
 * A334614  a(n) = pi(prime(n) - n) + n, where pi is the prime counting
 *          function.
 * https://oeis.org/A334614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi( prime(n) – n ) + n
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th term of OEIS sequence A334614. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Static state: prime sieve storage */
static idx_t current_prime_limit = 0;   /* maximum number up to which primes were sieved */
static uint64_t *prime_list   = NULL;   /* sorted primes, 0‑indexed */
static size_t   prime_capacity = 0;     /* allocated capacity of prime_list */
static size_t   prime_count     = 0;     /* actual number of primes stored */

/* Estimate a safe upper bound for the n-th prime.  For n ≥ 6,
   p_n < n (ln n + ln ln n).  Add a small safety margin. */
static inline idx_t estimate_prime_limit(idx_t n)
{
    if (n < 6)
        return (idx_t)15;
    double nn = (double)n;
    double lim = nn * (log(nn) + log(log(nn)));
    return (idx_t)(lim + 10.0);          /* safety margin */
}

/* Grow the prime_list array if necessary. */
static void ensure_prime_capacity(size_t needed)
{
    if (prime_capacity >= needed)
        return;
    size_t new_cap = prime_capacity == 0 ? 32 : prime_capacity * 2;
    while (new_cap < needed)
        new_cap *= 2;
    uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!tmp)
        abort();
    prime_list = tmp;
    prime_capacity = new_cap;
}

/* Build (or extend) the sieve up to 'limit' and fill prime_list. */
static void extend_sieve(idx_t limit)
{
    if (limit <= current_prime_limit)
        return;                         /* already sufficient */

    /* Allocate a bit array for the sieve. */
    uint8_t *is_prime = (uint8_t *)calloc((size_t)(limit + 1), sizeof(uint8_t));
    if (!is_prime)
        abort();

    /* Mark all as prime initially (except 0 and 1). */
    for (idx_t i = 2; i <= limit; ++i)
        is_prime[i] = 1;

    /* Sieve of Eratosthenes. */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* Count primes and copy to prime_list. */
    size_t pcnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            if (pcnt == prime_capacity)
                ensure_prime_capacity(pcnt + 1);
            prime_list[pcnt++] = (uint64_t)i;
        }
    }

    prime_count = pcnt;
    current_prime_limit = limit;
    free(is_prime);
}

/* Return the number of primes ≤ x (π(x)) using binary search. */
static inline idx_t pi_leq(idx_t x)
{
    idx_t lo = 0, hi = (idx_t)prime_count;
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        if ((idx_t)prime_list[mid] <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

/* Sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;        /* undefined for n = 0; return 0 per convention */

    /* Ensure we have enough primes to answer this query. */
    idx_t req_limit = estimate_prime_limit(n);
    extend_sieve(req_limit);

    /* Retrieve the n-th prime (1‑based). */
    if ((size_t)n > prime_count)
        abort();               /* safety check: not enough primes generated */

    idx_t pn = (idx_t)prime_list[n - 1];

    /* Compute π( pn – n ). */
    idx_t k = pn - n;            /* k ≥ 0 for all n ≥ 1 */
    idx_t pi_k = pi_leq(k);

    /* Return the sequence value: π(pn – n) + n. */
    return (seq_t)(pi_k + n);
}
