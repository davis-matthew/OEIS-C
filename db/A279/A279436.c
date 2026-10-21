/*
 * A279436  Number of nonprimes less than or equal to n that do not divide n.
 * https://oeis.org/A279436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - π(n) - d(n) + ω(n) where π(n) is the prime counting
 *            function, d(n) is the number of positive divisors of n, ω(n) is
 *            the number of distinct prime factors of n (OEIS A279436)
 * Range:     u8 n <= 325, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* --- global state for the sieve ------------------------------------------- */
static bool   *is_composite = NULL;   /* true if composite */
static uint32_t *spf        = NULL;   /* smallest prime factor */
static uint32_t *primes     = NULL;   /* list of primes */
static size_t   *pi_arr     = NULL;   /* π(n): cumulative prime count */
static size_t    cur_max    = 0;      /* current upper limit of sieve */

/* Recompute the sieve up to N (inclusive) whenever required. */
static void
ensure_sieve(size_t N)
{
    if (N <= cur_max)
        return;                /* already sufficient */

    /* free old arrays (if any) */
    free(is_composite);
    free(spf);
    free(primes);
    free(pi_arr);

    /* allocate new arrays */
    size_t sz = N + 1;
    is_composite = (bool*)calloc(sz, sizeof(bool));
    spf          = (uint32_t*)calloc(sz, sizeof(uint32_t));
    primes       = (uint32_t*)calloc(sz, sizeof(uint32_t));
    pi_arr       = (size_t*)calloc(sz, sizeof(size_t));

    if (!is_composite || !spf || !primes || !pi_arr) {
        /* Allocation failed – abort (should not happen for N ≤ 10000) */
        exit(EXIT_FAILURE);
    }

    /* Linear sieve algorithm to fill is_composite, spf, primes */
    size_t prime_cnt = 0;
    for (size_t i = 2; i <= N; ++i) {
        if (!is_composite[i]) {
            spf[i] = (uint32_t)i;
            primes[prime_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < prime_cnt && primes[j] <= spf[i] &&
                         (size_t)i * primes[j] <= N; ++j) {
            size_t m = i * primes[j];
            is_composite[m] = true;
            spf[m] = primes[j];
        }
    }

    /* Build π(n) array */
    size_t pi_count = 0;
    pi_arr[0] = 0;
    for (size_t i = 1; i <= N; ++i) {
        if (i >= 2 && !is_composite[i]) {
            ++pi_count;
        }
        pi_arr[i] = pi_count;
    }

    cur_max = N;
}

/* Compute a(n) for any n ≥ 1 */
seq_t
a(idx_t n)
{
    if (n == (idx_t)0)          /* undefined, but return 0 */
        return (seq_t)0;

    size_t N = (size_t)n;
    ensure_sieve(N);

    /* π(n) */
    size_t pi_val = pi_arr[N];

    /* Factorization using spf to get d(n) and ω(n) */
    idx_t   tmp       = n;
    seq_t   dcount    = (seq_t)1;    /* divisor count */
    size_t  omega_cnt = 0;           /* distinct prime factors */

    while (tmp > 1) {
        uint32_t p = spf[(size_t)tmp];
        size_t   e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        dcount   *= (seq_t)(e + 1);
        ++omega_cnt;
    }

    /* a(n) = n - π(n) - d(n) + ω(n) */
    seq_t result = (seq_t)n - (seq_t)pi_val - dcount + (seq_t)omega_cnt;
    return result;
}
