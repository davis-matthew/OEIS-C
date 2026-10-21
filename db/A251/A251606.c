/*
 * A251606  Let {d(i)}, i=1..q, be the set of the q prime distinct divisors of
 *          a number n, and consider the alternating sum s = Sum_{i=1..q}
 *          i*((-1)^(i+1))/d(i) and the product p = Product_{i=1..q} d(i)
 *          respectively. The sequence lists the composite numbers n such that
 *          s = 1/p.
 * https://oeis.org/A251606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th composite integer whose prime divisors
 *            d1<d2<…<dq satisfy Σ_{i=1..q} i·(-1)^{i+1}·(product(d_j)/d_i) =
 *            1 (product(d_j)=∏d_j)
 * Range:     u8 n <= 32, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --------------------------------------------------------------- */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* --------------------------------------------------------------- */
/* Prime number cache                                              */
static uint64_t *prime_list  = NULL;   /* array of primes */
static size_t    prime_cnt   = 0;      /* number of primes stored */
static size_t    prime_cap   = 0;      /* allocated capacity */
static uint64_t  prime_limit = 1;      /* largest value up to which primes are known */

/* Grow the prime array if necessary */
static void prime_resize(size_t new_cap)
{
    uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    prime_list = tmp;
    prime_cap  = new_cap;
}

/* Make sure primes are known up to 'limit' (inclusive) */
static void ensure_primes_upto(uint64_t limit)
{
    if (limit <= prime_limit) return;
    /* ensure the prime list is large enough */
    if (!prime_list) prime_resize(128);

    for (uint64_t cand = prime_limit + 1; cand <= limit; ++cand) {
        bool is_p = true;
        uint64_t r = (uint64_t) sqrt((long double)cand);
        for (size_t i = 0; i < prime_cnt && prime_list[i] <= r; ++i) {
            if (cand % prime_list[i] == 0) { is_p = false; break; }
        }
        if (is_p) {
            if (prime_cnt == prime_cap) prime_resize(prime_cap ? prime_cap * 2 : 128);
            prime_list[prime_cnt++] = cand;
        }
    }
    prime_limit = limit;
}

/* Simple primality test using the cached primes */
static bool is_prime(uint64_t n)
{
    if (n < 2) return false;
    uint64_t limit = (uint64_t) sqrt((long double)n);
    ensure_primes_upto(limit);
    for (size_t i = 0; i < prime_cnt && prime_list[i] <= limit; ++i) {
        if (n % prime_list[i] == 0) return false;
    }
    return true;
}

/* --------------------------------------------------------------- */
/* Compute the set of distinct prime divisors of 'n'.              */
/* The primes are written in ascending order into 'primes_out'    */
/* and the number is returned in *cnt_out.                         */
static void factor_distinct(uint64_t n, uint64_t *primes_out, size_t *cnt_out)
{
    uint64_t tmp = n;
    size_t   idx = 0;
    uint64_t r   = (uint64_t) sqrt((long double)tmp);
    ensure_primes_upto(r);

    for (size_t i = 0; i < prime_cnt && prime_list[i] <= r; ++i) {
        uint64_t p = prime_list[i];
        if (tmp % p == 0) {
            primes_out[idx++] = p;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) {
        /* tmp is a prime larger than sqrt(original n) */
        primes_out[idx++] = tmp;
    }
    *cnt_out = idx;
}

/* --------------------------------------------------------------- */
/* Test whether a composite integer 'x' satisfies the defining     */
/* condition.  The implementation follows the idea described      */
/* above: compute product of distinct primes, then the alternating */
/* sum of terms using two accumulators 'pos' and 'neg'.             */
static bool satisfies_condition(uint64_t x)
{
    /* x is assumed composite (checked by caller) */
    uint64_t primes_buf[64];
    size_t   prime_cnt_local;

    factor_distinct(x, primes_buf, &prime_cnt_local);

    /* product of distinct primes (as seq_t) */
    seq_t prod = 1;
    for (size_t i = 0; i < prime_cnt_local; ++i) prod *= (seq_t)primes_buf[i];

    /* accumulators for positive and negative parts of sum */
    seq_t pos = 0;
    seq_t neg = 0;

    for (size_t i = 0; i < prime_cnt_local; ++i) {
        uint64_t d   = primes_buf[i];
        seq_t   term = prod / (seq_t)d;       /* p/d_i */
        term *= (seq_t)(i + 1);               /* * (i+1) */
        if ((i & 1) == 0) {                  /* i even -> positive sign */
            pos += term;
        } else {                              /* i odd -> negative sign */
            neg += term;
        }
    }

    /* Now we need pos - neg == 1  (and pos > neg) */
    if (pos <= neg) return false;
    seq_t diff = pos - neg;
    return diff == 1;
}

/* --------------------------------------------------------------- */
/* Main sequence function                                          */
seq_t a(idx_t n)
{
    /* n is >= 1 for real usage; return 0 for n==0 */
    if (n == 0) return (seq_t)0;

    idx_t  found = 0;
    uint64_t x   = 2;  /* start checking from 2 */

    while (found < n) {
        ++x;
        if (is_prime(x)) continue;            /* skip primes */
        if (satisfies_condition(x)) {
            ++found;
            if (found == n) return (seq_t)x;
        }
    }
    return (seq_t)0;   /* never reached */
}
