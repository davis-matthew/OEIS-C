/*
 * A303545  For any n > 0 and prime number p, let d_p(n) be the distance from
 *          n to the nearest p-smooth number; a(n) = Sum_{i prime} d_i(n).
 * https://oeis.org/A303545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = sum over all primes p of d_p(n), where d_p(n) is the
 *            distance from n to the nearest p‑smooth number. For a given n
 *            the summands for primes p > A006530(n) (the largest prime factor
 *            of n) are zero, so the sum is finite
 * Range:     u8 n <= 612, u16 n <= 9675, u32 n <= 9675, u64 n <= 9675,
 *            u128 n <= 9675, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: iterate over primes p ≤ largest_principal_factor(n).
   For each p find the smallest distance d to a p‑smooth integer by
   outward search.  The function is memoised per n via a static prime
   list which grows as needed. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static storage for the list of primes needed by the algorithm. */
static uint64_t *prime_list = NULL;   /* array of primes */
static size_t     prime_cnt   = 0;     /* number of primes stored */
static idx_t      prime_max   = 0;     /* maximum n for which primes are built */

/* Build the list of all primes up to 'limit' inclusive.
   The list is expanded only when 'limit' exceeds the current bound. */
static void build_primes_up_to(idx_t limit)
{
    if (limit <= prime_max)
        return;                         /* already built */

    idx_t n = limit;
    bool *is_prime = (bool *)calloc((size_t)n + 1, sizeof(bool));
    if (!is_prime)
        return;                         /* out of memory – leave as is */

    for (size_t i = 2; i <= (size_t)n; ++i)
        is_prime[i] = true;

    for (size_t i = 2; i * i <= (size_t)n; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= (size_t)n; j += i)
                is_prime[j] = false;

    /* Build new prime list. */
    uint64_t *new_list = (uint64_t *)malloc((size_t)n * sizeof(uint64_t));
    size_t new_cnt = 0;
    for (size_t i = 2; i <= (size_t)n; ++i)
        if (is_prime[i])
            new_list[new_cnt++] = (uint64_t)i;

    free(prime_list);
    free(is_prime);
    prime_list = new_list;
    prime_cnt   = new_cnt;
    prime_max   = limit;
}

/* Return true iff m is p‑smooth (all prime factors ≤ p). */
static bool is_psmooth(uint64_t m, uint64_t p)
{
    if (m <= 1)
        return true;

    for (size_t i = 0; i < prime_cnt && prime_list[i] <= p; ++i)
    {
        uint64_t q = prime_list[i];
        while (m % q == 0)
        {
            m /= q;
            if (m == 1)
                return true;
        }
    }
    return m == 1;      /* remaining factor (if any) is > p */
}

/* Return the largest prime divisor of n; for n ≤ 1 return 1. */
static uint64_t largest_prime_factor(uint64_t n)
{
    if (n <= 1)
        return 1;

    uint64_t best = 1;
    uint64_t tmp  = n;
    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= tmp; ++i)
    {
        uint64_t p = prime_list[i];
        if (tmp % p == 0)
        {
            best = p;
            while (tmp % p == 0)
                tmp /= p;
        }
    }
    if (tmp > 1)                     /* remaining prime divisor */
        best = tmp;

    return best;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Ensure primes up to n are available. */
    build_primes_up_to(n);

    uint64_t N = (uint64_t)n;
    uint64_t largest_pf = largest_prime_factor(N);

    seq_t total = 0;

    for (size_t idx = 0; idx < prime_cnt; ++idx)
    {
        uint64_t p = prime_list[idx];
        if (p > largest_pf)
            break;                     /* larger primes give d_p(n)=0 */

        if (is_psmooth(N, p))
            continue;                  /* d_p(n) = 0 */

        /* Search outward for nearest p‑smooth number. */
        uint64_t dist = 1;
        while (true)
        {
            bool below = (dist <= N) && is_psmooth(N - dist, p);
            bool above = is_psmooth(N + dist, p);
            if (below || above)
                break;
            ++dist;
        }
        total += (seq_t)dist;
    }
    return total;
}
