/*
 * A279536  Count the squarefree numbers appearing in each interval [p,q]
 *          where (p,q) is a Goldbach partition of 2n such that all primes
 *          from p to q (inclusive) appear as a part in some Goldbach
 *          partition of p+q = 2n, and then add the results.
 * https://oeis.org/A279536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = \sum_{i=3..n} (A010051(i) * A010051(2n-i) *
 *            (\sum_{j=i..2n-i} A008683(j)^2) * \prod_{k=i..n} (1 -
 *            |A010051(k) - A010051(2n-k)|)) where A010051 is the prime
 *            indicator and A008683 is the Möbius function
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------- static data for sieving and prefix sums ------------- */
static idx_t  current_limit = 0;          /* 2 * max n processed so far    */
static unsigned char *isPrime  = NULL;     /* 1 if prime, 0 otherwise       */
static int8_t      *mu        = NULL;      /* Möbius function values        */
static idx_t      *prefix_sf = NULL;       /* prefix of mu^2 (squarefree)   */
static unsigned char *isComposite = NULL; /* helper for linear sieve      */

static unsigned char *suffix_good = NULL;  /* suffix product of equal primality  */
static idx_t  suffix_cap = 0;              /* size of allocated suffix array    */
static size_t primes_cap = 0;              /* allocated space for primes        */
static size_t primes_cnt = 0;
static idx_t *primes_arr = NULL;           /* list of primes                    */

/* ---------- helper: rebuild sieve and prefix up to new_limit ---------- */
static void ensure_limit(idx_t n)
{
    idx_t new_limit = 2 * n;
    if (new_limit <= current_limit)
        return; /* already enough */

    /* free old arrays if any and allocate new ones */
    /* isPrime, mu, prefix_sf, isComposite */
    size_t len = (size_t)new_limit + 1;
    isComposite = (unsigned char*)realloc(isComposite, len * sizeof(unsigned char));
    mu         = (int8_t*)realloc(mu,         len * sizeof(int8_t));
    isPrime    = (unsigned char*)realloc(isPrime,    len * sizeof(unsigned char));
    prefix_sf  = (idx_t*)realloc(prefix_sf,  len * sizeof(idx_t));

    /* linear sieve with Möbius */
    for (size_t i = 0; i < len; ++i) {
        isComposite[i] = 0;
    }
    mu[1] = 1;
    primes_cnt = 0;
    if (primes_cap < (size_t)new_limit / 10 + 10) {
        primes_cap = (size_t)new_limit / 10 + 10;
        primes_arr = (idx_t*)realloc(primes_arr, primes_cap * sizeof(idx_t));
    }

    for (idx_t i = 2; i <= new_limit; ++i) {
        if (!isComposite[i]) {
            if (primes_cnt >= primes_cap) {
                primes_cap = primes_cap * 2 + 10;
                primes_arr = (idx_t*)realloc(primes_arr, primes_cap * sizeof(idx_t));
            }
            primes_arr[primes_cnt++] = i;
            mu[i] = -1;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            idx_t p = primes_arr[j];
            idx_t ip = i * p;
            if (ip > new_limit)
                break;
            isComposite[ip] = 1;
            if (i % p == 0) {
                mu[ip] = 0;
                break;
            } else {
                mu[ip] = -mu[i];
            }
        }
    }

    /* set prime flags (0,1 are not prime) */
    isPrime[0] = isPrime[1] = 0;
    for (idx_t i = 2; i <= new_limit; ++i) {
        isPrime[i] = !isComposite[i];
    }

    /* build prefix of squarefree counts */
    prefix_sf[0] = 0;
    for (idx_t i = 1; i <= new_limit; ++i) {
        prefix_sf[i] = prefix_sf[i-1] + (mu[i] != 0 ? 1 : 0);
    }

    current_limit = new_limit;
}

/* ---------- main sequence function ----------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Ensure we have data for 2*n */
    ensure_limit(n);
    idx_t limit = 2 * n;

    /* Ensure suffix_good array large enough */
    if (suffix_cap < (idx_t)n + 2) {
        suffix_good = (unsigned char*)realloc(suffix_good,
                                             ((size_t)n + 2) * sizeof(unsigned char));
        suffix_cap = (idx_t)n + 2;
    }

    /* compute suffix_good: product over k = i..n of (primality equality) */
    suffix_good[n+1] = 1;
    for (idx_t k = n; k >= 3; --k) {
        unsigned char same = (isPrime[k] == isPrime[limit - k]) ? 1 : 0;
        suffix_good[k] = suffix_good[k+1] & same;
    }

    seq_t total = (seq_t)0;
    for (idx_t i = 3; i <= n; ++i) {
        if (!isPrime[i] || !isPrime[limit - i] || !suffix_good[i])
            continue;
        idx_t cnt_sf = prefix_sf[limit - i] - prefix_sf[i-1];
        total += (seq_t)cnt_sf;
    }

    return total;
}
