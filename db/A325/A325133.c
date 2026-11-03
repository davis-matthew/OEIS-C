/*
 * A325133  Heinz number of the integer partition obtained by removing the
 *          inner lining, or, equivalently, the largest hook, of the integer
 *          partition with Heinz number n.
 * https://oeis.org/A325133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A064989(A052126(n)) = A052126(A064989(n))
 * Range:     u8 n <= 6858, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 121 terms through n = 9316 (2026-11-01)
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the Heinz number of the partition obtained by removing the
   largest hook from the partition with Heinz number n. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"

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

static uint64_t *primes = NULL;   /* array of primes up to the current limit */
static size_t primes_cnt = 0;      /* number of primes stored */
static idx_t primes_limit = 0;     /* largest value for which primes are generated */

/* Ensure that the list of primes contains all primes ≤ limit. */
static void ensure_primes(idx_t limit) {
    if (primes == NULL || limit > primes_limit) {
        free(primes);
        size_t cnt;
        uint64_t *new_primes = NULL;
        build_primes_upto(limit, &new_primes, &cnt);
        primes = new_primes;
        primes_cnt = cnt;
        primes_limit = limit;
    }
}

/* Binary search for the index of a prime number p in the primes array. */
static idx_t prime_index(uint64_t p) {
    idx_t lo = 1, hi = primes_cnt;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        uint64_t val = primes[mid - 1];
        if (val == p) return mid;
        if (val < p)   lo = mid + 1;
        else           hi = mid - 1;
    }
    return 0; /* should never happen for a prime factor of n */
}

seq_t a(idx_t n) {
    /* Prepare primes up to n for factorization and index lookup. */
    ensure_primes(n);

    seq_t result = (seq_t)1;
    uint64_t m = (uint64_t)n;
    if (m <= 1) return result;

    /* Store the partition parts (prime indices) in a fixed array. */
    idx_t parts[256];
    size_t parts_cnt = 0;

    /* Factor n and collect indices of the primes. */
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= m; ++i) {
        uint64_t p = primes[i];
        while (m % p == 0) {
            parts[parts_cnt++] = (idx_t)(i + 1); /* prime index is i+1 */
            m /= p;
        }
    }
    if (m > 1) { /* remaining prime factor > sqrt(original n) */
        parts[parts_cnt++] = prime_index(m);
    }

    /* Find the largest part; skip exactly one instance of it. */
    idx_t max_part = 0;
    for (size_t i = 0; i < parts_cnt; ++i)
        if (parts[i] > max_part) max_part = parts[i];

    bool skipped = false;
    for (size_t i = 0; i < parts_cnt; ++i) {
        idx_t part = parts[i];
        if (!skipped && part == max_part) {
            skipped = true;
            continue; /* remove the largest part */
        }
        if (part <= 1) continue; /* part-1 == 0 -> discard */
        idx_t new_part = part - 1;
        seq_t prime_val = (seq_t)get_prime(new_part);
        result *= prime_val; /* multiply the new prime values */
    }

    return result;
}
