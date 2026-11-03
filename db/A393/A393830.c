/*
 * A393830  a(n) is the difference between the nearest upper and lower odd
 *          semiprime neighbors of prime(n), divided by 2.
 * https://oeis.org/A393830
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (A393828(n) - A393829(n))/2 where A393828(n) is the
 *            nearest upper odd semiprime neighbor of the n‑th prime, and
 *            A393829(n) is the nearest lower odd semiprime neighbor. The
 *            implementation searches for the neighbors by trial, using the
 *            supplied factor_u64 and get_prime utilities
 * Range:     u8 n <= 54, u16 n <= 7176, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 104 terms through n = 8296 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

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

static bool is_odd_semiprime_uint64(uint64_t x)
{
    /* even numbers or too small cannot be odd semiprime */
    if ((x & 1ULL) == 0ULL || x < 9ULL)
        return false;

    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(x, primes, exps, &cnt);

    unsigned total = 0;
    for (size_t i = 0; i < cnt; ++i)
        total += (unsigned)exps[i];

    return total == 2;
}

seq_t a(idx_t n)
{
    /* nth prime (1‑based).  n >= 5 according to offset, but the
       function works for any n for which get_prime is defined. */
    uint32_t p32 = get_prime(n);
    seq_t p = (seq_t)p32;

    /* Search for nearest lower odd semiprime */
    uint64_t lower = 0;
    for (uint64_t x = (uint64_t)p - 1; x >= 9ULL; --x) {
        if (is_odd_semiprime_uint64(x)) {
            lower = x;
            break;
        }
    }

    /* Search for nearest upper odd semiprime */
    uint64_t upper = 0;
    for (uint64_t x = (uint64_t)p + 1;; ++x) {
        if (is_odd_semiprime_uint64(x)) {
            upper = x;
            break;
        }
    }

    /* The difference between two odd numbers is even, so division is safe. */
    seq_t result = (seq_t)((upper - lower) / 2ULL);
    return result;
}
