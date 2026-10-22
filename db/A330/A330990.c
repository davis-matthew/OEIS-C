/*
 * A330990  Numbers whose inverse prime shadow (A181821) has its number of
 *          factorizations into factors > 1 (A001055) equal to a power of 2
 *          (A000079).
 * https://oeis.org/A330990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) is the sequence of positive integers whose inverse prime *
 *            shadow’s number of factorizations into factors > 1 is a power of
 *            two. * The implementation computes a(n) on demand by scanning
 *            natural numbers, * evaluating the inverse prime shadow, counting
 *            its multiplicative * factorizations, and...
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   build_primes_upto, cmp_idx_desc, floor_sqrt, is_power_of_two,
 *            pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_power_of_two.h"
#include "../utils/floor_sqrt.h"
#include "../utils/pow_seq.h"
#include "../utils/cmp_idx_desc.h"

/* ----------------------------------------------------------------------
 * Global data: primes list and sequence cache
 * ---------------------------------------------------------------------- */
static uint64_t *primes = NULL;        /* array of primes in ascending order */
static size_t primes_count = 0;        /* number of primes */
static seq_t *seq_cache = NULL;       /* cache of computed a(n) values */
static size_t seq_len = 0;             /* number of values cached */
static size_t seq_cap = 0;             /* allocated capacity */
static idx_t next_candidate = 1;       /* next natural number to test */

/* ----------------------------------------------------------------------
 * Helper: binary search to find the 1‑based index of a prime in the array
 * ---------------------------------------------------------------------- */
static size_t get_prime_index(uint64_t p)
{
    size_t lo = 0, hi = primes_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (lo < primes_count && primes[lo] == p) ? lo + 1 : 0; /* 0 if not found */
}

/* ----------------------------------------------------------------------
 * Inverse prime shadow A181821(n)
 * ---------------------------------------------------------------------- */
static seq_t inverse_prime_shadow(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    uint64_t tmp = n;
    /* temporary array to hold prime indices (exponents) */
    idx_t indices[256];
    int idx_cnt = 0;

    /* trial division using the precomputed primes */
    for (size_t pi = 0; pi < primes_count && primes[pi] * primes[pi] <= tmp; ++pi) {
        uint64_t p = primes[pi];
        if (tmp % p == 0) {
            int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            /* prime index is (pi + 1) */
            for (int k = 0; k < exp; ++k)
                indices[idx_cnt++] = (idx_t)(pi + 1);
        }
    }
    if (tmp > 1) {                /* tmp is a remaining prime factor */
        size_t idx = get_prime_index(tmp);
        if (idx == 0)             /* should not happen */
            return (seq_t)1;
        indices[idx_cnt++] = (idx_t)idx;
    }

    /* sort indices descending so that the largest exponent gets the
       smallest prime, producing the minimal number */
    if (idx_cnt > 1)
        qsort(indices, (size_t)idx_cnt, sizeof(idx_t), cmp_idx_desc);

    seq_t result = (seq_t)1;
    for (int i = 0; i < idx_cnt; ++i) {
        uint64_t prime_val = primes[i];          /* i-th prime (0‑based) */
        result *= pow_seq((seq_t)prime_val, indices[i]);
    }
    return result;
}

/* ----------------------------------------------------------------------
 * Recursive counting of unordered factorizations of n (>1) into factors >1
 * ---------------------------------------------------------------------- */
static seq_t count_factorizations_rec(seq_t n, seq_t min_factor)
{
    seq_t count = 0;
    seq_t upper = floor_sqrt(n);
    for (seq_t f = min_factor; f <= upper; ++f) {
        if (n % f == 0) {
            count += count_factorizations_rec(n / f, f);
        }
    }
    return count + 1;   /* include the factorization consisting of n itself */
}

static seq_t count_factorizations(seq_t n)
{
    return count_factorizations_rec(n, (seq_t)2);
}

/* ----------------------------------------------------------------------
 * Main sequence function a(n)
 * ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n==0 */

    /* initialize on first call */
    if (seq_cache == NULL) {
        /* choose a generous upper bound for the prime sieve */
        idx_t sieve_limit = 2000000;               /* plenty for n up to ~10^5 */
        uint64_t *tmp_primes = NULL;
        size_t tmp_cnt = 0;
        build_primes_upto(sieve_limit, &tmp_primes, &tmp_cnt);
        primes = tmp_primes;
        primes_count = tmp_cnt;

        seq_cache = NULL;
        seq_len = 0;
        seq_cap = 0;
        next_candidate = 1;
    }

    while (seq_len < n) {
        /* Evaluate candidate number */
        seq_t shadow = inverse_prime_shadow(next_candidate);
        seq_t fac_cnt = count_factorizations(shadow);
        if (is_power_of_two(fac_cnt)) {
            /* store in cache */
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 16;
                seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
            }
            seq_cache[seq_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    /* Return the requested term */
    return seq_cache[n - 1];
}
