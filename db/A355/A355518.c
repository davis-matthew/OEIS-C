/*
 * A355518  Primes that cannot be represented as 2*p - q where p, q and 2*p^2
 *          - q^2 are prime.
 * https://oeis.org/A355518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth prime that cannot be expressed as 2*p - q where
 *            p and q are primes and 2*p^2 - q^2 is also prime
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/ensure_capacity.h"
#include "../utils/is_prime_big.h"

/*-----------------------------------------------------------------------------*/
/* Global state                                                          */
/*-----------------------------------------------------------------------------*/
static uint64_t *primes = NULL;          /* list of all primes found */
static size_t primes_cnt = 0;            /* number of primes stored */
static size_t primes_cap = 0;            /* capacity of primes array */
static uint64_t next_prime_candidate = 3;/* next odd number to test */

static seq_t *seq_arr = NULL;           /* sequence values */
static size_t seq_cnt = 0;              /* number of sequence elements */
static size_t seq_cap = 0;              /* capacity of seq_arr */

static idx_t prime_test_index = 0;      /* index of next prime r to test */

/*-----------------------------------------------------------------------------*/
/* Utility functions                                                    */
/*-----------------------------------------------------------------------------*/
static void add_prime(uint64_t p)
{
    ensure_capacity(primes_cnt + 1, &primes_cap, (void **)&primes,
                    sizeof(uint64_t));
    primes[primes_cnt++] = p;
}

static void add_seq(seq_t v)
{
    ensure_capacity(seq_cnt + 1, &seq_cap, (void **)&seq_arr,
                    sizeof(seq_t));
    seq_arr[seq_cnt++] = v;
}

/* Lower bound: first index i with primes[i] >= val */
static size_t lower_bound_prime(idx_t val)
{
    size_t lo = 0, hi = primes_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

/* Ensure that all primes <= bound are generated. */
static void ensure_primes_up_to(idx_t bound)
{
    if (primes_cnt == 0) {
        add_prime(2);
        next_prime_candidate = 3;
    }
    if (primes[primes_cnt - 1] >= bound)
        return;

    for ( ; next_prime_candidate <= bound; next_prime_candidate += 2 ) {
        bool isprime = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > next_prime_candidate)
                break;
            if (next_prime_candidate % p == 0) {
                isprime = false;
                break;
            }
        }
        if (isprime)
            add_prime(next_prime_candidate);
    }
}

/*-----------------------------------------------------------------------------*/
/* Sequence generation logic                                           */
/*-----------------------------------------------------------------------------*/
static void compute_terms_up_to(idx_t target_n)
{
    /* Ensure at least one prime exists */
    ensure_primes_up_to(2);

    /* Grow primes list when necessary */
    while (seq_cnt < target_n) {
        /* Extend primes until we have one to test */
        while (prime_test_index >= primes_cnt) {
            idx_t new_limit = primes_cnt ? (idx_t)(primes[primes_cnt - 1] * 2) : 17;
            ensure_primes_up_to(new_limit);
        }

        uint64_t r = primes[prime_test_index];          /* candidate prime r */
        idx_t p_start = (r + 3) / 2;                    /* ceil((r+2)/2) */
        const long double GAMMA = 1.70710678118654752440L; /* 1 + sqrt(2)/2 */
        idx_t p_max   = (idx_t)((long double)r * GAMMA);

        /* If bound is exceeded, r automatically qualifies */
        if (p_max < p_start) {
            add_seq((seq_t)r);
            ++prime_test_index;
            continue;
        }

        /* Make sure we have primes up to p_max */
        ensure_primes_up_to(p_max);

        size_t pi = lower_bound_prime(p_start);
        bool found = false;
        for ( ; pi < primes_cnt; ++pi) {
            uint64_t p = primes[pi];
            if (p > p_max)
                break;
            uint64_t q = 2 * p - r;
            if (!is_prime_big((seq_t)q))
                continue;
            seq_t p_seq = (seq_t)p;
            seq_t q_seq = (seq_t)q;
            seq_t w = 2 * (p_seq * p_seq) - (q_seq * q_seq);
            if (w == 0)   /* should not happen due to bound */
                continue;
            if (is_prime_big(w)) {
                found = true;
                break;
            }
        }
        if (!found)
            add_seq((seq_t)r);
        ++prime_test_index;
    }
}

/*-----------------------------------------------------------------------------*/
/* Public interface                                                    */
/*-----------------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* offset = 1, undefined for n==0 */

    /* Lazily compute terms up to n */
    static idx_t last_computed = 0;
    if (n > last_computed)
        compute_terms_up_to(n), last_computed = n;

    return seq_arr[n - 1];
}
