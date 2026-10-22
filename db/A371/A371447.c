/*
 * A371447  Numbers whose binary indices of prime indices cover an initial
 *          interval of positive integers.
 * https://oeis.org/A371447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer m such that the set of binary
 *            positions of the prime indices of m forms a contiguous interval
 *            starting at 1. For example, for m = 6 (prime indices 1,2) the
 *            binary indices are {1,2} – a contiguous interval — so 6 is
 *            accepted
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The implementation keeps a cache of prime numbers and their 1‑based
    indices, and a vector of already found terms.  For each candidate
    integer it is factorised by trial division using the cached primes,
    and the binary‑index mask of all distinct prime indices is built.
    A number is accepted iff that mask is of the form 2^k‑1 (no gaps). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

static uint64_t *prime_list = NULL;     /* primes in ascending order   */
static size_t    prime_count = 0;        /* number of primes            */
static uint32_t *prime_index = NULL;    /* prime_index[p] = 1‑based index of p  */
static uint64_t  prime_limit = 0;       /* greatest prime stored       */

static seq_t *seq_vals = NULL;          /* cached terms, 1‑based       */
static size_t seq_cap = 0;              /* allocated capacity          */
static size_t seq_len = 0;              /* number of terms stored      */
static uint64_t last_checked = 0;       /* last integer examined       */

/* ensure that all primes up to 'limit' are known and prime_index[] is valid */
static void ensure_prime_limit(uint64_t limit)
{
    if (prime_limit >= limit)
        return;

    free(prime_list);
    free(prime_index);

    size_t cnt;
    build_primes_upto(limit, &prime_list, &cnt);
    prime_count = cnt;
    prime_limit = limit;

    prime_index = (uint32_t *)calloc((size_t)prime_limit + 1, sizeof(uint32_t));
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        prime_index[p] = (uint32_t)(i + 1);   /* 1‑based index */
    }
}

/* true if m satisfies the defining property */
static bool is_good_candidate(uint64_t m)
{
    if (m == 1)                /* empty set covers empty interval */
        return true;

    /* prime factorisation by trial division using prime_list */
    uint64_t temp = m;
    uint64_t mask = 0;          /* bits representing binary indices */

    for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] * prime_list[i] <= temp; ++i) {
        uint64_t p = prime_list[i];
        if (temp % p == 0) {
            uint32_t idx = prime_index[p];  /* prime index (1‑based) */
            /* add binary indices of idx to mask */
            uint64_t bits = 0;
            uint64_t t = idx;
            uint64_t pos = 0;
            while (t) {
                if (t & 1ULL)
                    bits |= (1ULL << pos);
                t >>= 1;
                ++pos;
            }
            mask |= bits;
            while (temp % p == 0)
                temp /= p;
        }
    }

    if (temp > 1) {  /* remaining prime factor > sqrt(m) */
        if (temp > prime_limit)
            ensure_prime_limit(temp);
        uint32_t idx = prime_index[temp];
        uint64_t bits = 0;
        uint64_t t = idx;
        uint64_t pos = 0;
        while (t) {
            if (t & 1ULL)
                bits |= (1ULL << pos);
            t >>= 1;
            ++pos;
        }
        mask |= bits;
    }

    /* mask must be of the form 2^k - 1 (i.e., no gaps) */
    return (mask & (mask + 1)) == 0;
}

/* generate terms up to index n (inclusive) */
static void compute_terms_upto(idx_t n)
{
    /* initialise cache if first time */
    if (prime_limit == 0) {
        ensure_prime_limit(2);    /* at least the first prime 2 */
        if (seq_vals == NULL) {
            seq_cap = (size_t)n + 1;
            seq_vals = (seq_t *)calloc(seq_cap, sizeof(seq_t));
            seq_len = 0;
        }
    }

    /* enlarge term cache if necessary */
    if (seq_cap < (size_t)n + 1) {
        size_t new_cap = seq_cap > 0 ? seq_cap * 2 : 16;
        while (new_cap < (size_t)n + 1)
            new_cap *= 2;
        seq_vals = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }

    while (seq_len < (size_t)n) {
        ++last_checked;
        if ((uint64_t)last_checked > prime_limit)
            ensure_prime_limit((uint64_t)last_checked);

        if (is_good_candidate((uint64_t)last_checked)) {
            ++seq_len;
            seq_vals[seq_len] = (seq_t)last_checked;
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    compute_terms_upto(n);
    return seq_vals[(size_t)n];
}
