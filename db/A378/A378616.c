/*
 * A378616  Greatest non prime power <= prime(n).
 * https://oeis.org/A378616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = greatest integer ≤ prime(n) that is NOT a prime power. *
 *            The result is prime(n)−1 except when prime(n)−1 itself is a
 *            prime power * (those terms are listed in OEIS A159611)
 * Range:     u8 n <= 54, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
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

/* Test whether a 64‑bit integer is a prime power (p^k, k≥1).
 * Returns true for primes, powers of primes, and false otherwise.
 * 1 is not a prime power. */
static bool is_prime_power_uint64(uint64_t x)
{
    if (x <= 1)
        return false;

    uint64_t r = x;
    for (uint64_t p = 2; p * p <= r; ++p) {
        if (r % p == 0) {
            while (r % p == 0)
                r /= p;
            return r == 1;
        }
    }
    /* If we reach here, r has no divisor ≤ sqrt(x).  It is prime => prime power. */
    return true;
}

seq_t a(idx_t n)
{
    /* n ≥ 1 as per OEIS offset. */
    uint32_t pn = get_prime(n);              /* nth prime, fits in 32 bits. */
    seq_t cand = (seq_t)pn - 1;              /* start from one below the prime. */

    for (seq_t v = cand; v > 0; --v) {
        if (!is_prime_power_uint64((uint64_t)v))
            return v;
    }
    return 1; /* Never reached, but keeps compiler happy. */
}
