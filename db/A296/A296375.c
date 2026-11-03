/*
 * A296375  Least x such that sigma(x) = prime(n)+1, where sigma = A000203.
 * https://oeis.org/A296375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least x such that sigma(x) = prime(n)+1
 * Range:     u8 n <= 46, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9422 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

static bool initialized = false;
static size_t B = 200000;                 /* upper bound for precomputation      */
static seq_t *sigma_cache = NULL;          /* sigma values for 1..B                */
static seq_t *min_of_sigma = NULL;         /* smallest x for a given sigma value  */
static size_t max_sigma = 0;               /* maximum sigma value computed        */

/* simple divisor sum for a single number, using trial division */
static seq_t sigma_single(uint64_t n)
{
    if (n == 0) return 0;
    seq_t result = 1;
    uint64_t m = n;
    for (uint64_t p = 2; p * (uint64_t)p <= m; ++p) {
        if (m % p == 0) {
            seq_t term = 1;
            seq_t power = 1;
            while (m % p == 0) {
                m /= p;
                power *= (seq_t)p;
                term += power;
            }
            result *= term;
        }
    }
    if (m > 1) {                         /* last prime factor > sqrt(n)        */
        result *= ((seq_t)1 + (seq_t)m);
    }
    return result;
}

static void init_arrays(void)
{
    if (initialized) return;
    /* allocate sigma cache */
    size_t sz = B + 1;
    sigma_cache = mem_new(seq_t, sz);
    if (!sigma_cache) return;
    for (size_t i = 0; i < sz; ++i) sigma_cache[i] = 0;

    /* sieve for divisor sums */
    for (size_t i = 1; i <= B; ++i) {
        for (size_t j = i; j <= B; j += i) {
            sigma_cache[j] += (seq_t)i;
        }
    }

    /* determine maximum sigma value */
    max_sigma = 0;
    for (size_t i = 1; i <= B; ++i) {
        if (sigma_cache[i] > max_sigma) max_sigma = (size_t)sigma_cache[i];
    }

    /* allocate minimal x per sigma */
    min_of_sigma = mem_new(seq_t, max_sigma + 1);
    if (!min_of_sigma) return;

    for (size_t i = 1; i <= B; ++i) {
        size_t val = (size_t)sigma_cache[i];
        if (min_of_sigma[val] == 0) min_of_sigma[val] = (seq_t)i;
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    init_arrays();
    if (!initialized) return 0;          /* fallback in case of allocation failure */

    uint32_t prime32 = get_prime((idx_t)n);   /* prime(n), fits in 32‑bit */
    seq_t target = (seq_t)prime32 + (seq_t)1;

    if ((size_t)target <= max_sigma) {
        seq_t res = min_of_sigma[(size_t)target];
        if (res != 0) return res;
    }

    /* target not found within precomputed range: scan upwards */
    uint64_t start = B + 1;
    while (true) {
        seq_t val = sigma_single(start);
        if (val == target) return (seq_t)start;
        ++start;
    }
}
