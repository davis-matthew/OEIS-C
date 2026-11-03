/*
 * A321578  a(n) is the maximum value of k such that A007504(k) <= prime(n).
 * https://oeis.org/A321578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) := max k such that Σ_{i=1..k} prime(i) ≤ prime(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 90 terms through n = 9805 (2026-11-01)
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A321578: a(n) = largest k with A007504(k) ≤ prime(n) */
/* where A007504(k) = 2 + 3 + 5 + … + prime(k) (sum of first k primes). */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
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

static uint64_t *primes = NULL;      /* primes up to chosen limit */
static size_t    primes_cnt = 0;      /* number of primes stored */
static uint64_t *prefix_sum = NULL;   /* cumulative sums of primes */
static size_t    prefix_cnt = 0;      /* size of prefix_sum array */

/* Initialise the prime array and prefix sums once. */
static void init_primes(void) {
    if (primes != NULL)
        return;                     /* already initialised */

    /* 10000th prime is 104729; choose a safe limit slightly higher. */
    idx_t limit = 200000;             /* primes up to 200000 → ~17984 primes */

    /* Build primes up to 'limit' inclusive. */
    build_primes_upto(limit, &primes, &primes_cnt);

    /* In the unlikely event we did not get enough primes, enlarge. */
    if (primes_cnt < 10000) {
        free(primes);
        limit = 2000000;             /* enough for far more primes */
        build_primes_upto(limit, &primes, &primes_cnt);
    }

    /* Allocate and fill prefix sums. */
    prefix_cnt = primes_cnt;
    prefix_sum = (uint64_t *)malloc(sizeof(uint64_t) * prefix_cnt);
    uint64_t sum = 0;
    for (size_t i = 0; i < prefix_cnt; ++i) {
        sum += primes[i];
        prefix_sum[i] = sum;
    }
}

/* Return a(n).  idx_t n is 1‑based. */
seq_t a(idx_t n) {
    /* Ensure data structures are ready. */
    init_primes();

    /* Prime at index n (1‑based). */
    uint64_t prime_n = (uint64_t)get_prime(n);

    /* Binary search over prefix_sum to find largest k with sum <= prime_n. */
    idx_t lo = 0;
    idx_t hi = (idx_t)prefix_cnt - 1;
    idx_t answer = 0;                 /* will hold k (1‑based) */

    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (prefix_sum[mid] <= prime_n) {
            answer = mid + 1;         /* k = mid+1 */
            lo = mid + 1;
        } else {
            /* Since prefix_sum[0] <= prime_n for all n >= 1, mid is never 0 here. */
            hi = mid - 1;
        }
    }

    return (seq_t)answer;
}
