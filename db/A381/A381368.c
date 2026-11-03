/*
 * A381368  a(n) is the least k > n for which prime(n) + prime(k) is a square.
 * https://oeis.org/A381368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least k > n such that prime(n) + prime(k) is a perfect
 *            square
 * Range:     u8 n <= 7, u16 n <= 4469, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 8726 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

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

/* Integer square root for uint64_t, using binary search.
   Returns floor(sqrt(n)).  The value n is guaranteed to fit in uint64_t. */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = 4294967295ULL;   /* 2^32-1 is the largest possible root for uint64_t */
    while (low <= high) {
        uint64_t mid = (low + high) >> 1;
        uint64_t sq  = mid * mid;
        if (sq == n)
            return mid;
        if (sq < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;   /* high is the floor of sqrt(n) after loop exits */
}

seq_t a(idx_t n)
{
    /* prime(n) and prime(k) return 32‑bit integers; cast them to seq_t. */
    seq_t prime_n = (seq_t)get_prime(n);

    for (idx_t k = n + 1; ; ++k) {
        seq_t prime_k = (seq_t)get_prime(k);
        seq_t sum     = prime_n + prime_k;           /* fits comfortably in seq_t for n ≤ 10000 */
        uint64_t sum_u64 = (uint64_t)sum;            /* assume sum < 2^64, which holds for all required n */
        uint64_t s = isqrt_u64(sum_u64);

        if ((uint64_t)s * (uint64_t)s == sum_u64)
            return (seq_t)k;                         /* found the least k > n with the property */
    }
}
