/*
 * A284061  Triangle read by rows: T(n,k) = pi(prime(k) * prime(n+1)).
 * https://oeis.org/A284061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 2144, u32 n <= 11325, u64 n <= 11325,
 *            u128 n <= 11325, big any n
 * Validated: standard, pass, 81 terms through n = 9219 (2026-11-01)
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A284061: a(n) = pi(prime(k) * prime(r+1)),
    where r = smallest integer with r(r+1)/2 >= n
    and k = n - (r-1)r/2.
*/

#include <stdint.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/prime_pi.h"

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

static inline idx_t __row_for_n(idx_t n)
{
    // Initial estimate via the quadratic formula
    double d = sqrt((double)8 * (double)n + 1.0);
    idx_t r = (idx_t)((d - 1.0) / 2.0);

    // Correct the estimate
    while ((__uint128_t)r * (r + 1) / 2 < n) ++r;
    while (r > 0 && (__uint128_t)(r - 1) * r / 2 >= n) --r;
    return r;
}

static inline idx_t __k_for_n(idx_t n, idx_t r)
{
    idx_t prev_rows = (r - 1) * r / 2;
    return n - prev_rows;
}

seq_t a(idx_t n)
{
    /* a(1) = 3, a(2) = 4, a(3) = 6, a(4) = 6 ... */

    if (n == 0) return 0;   // offset 1 – not used, safe guard

    idx_t r = __row_for_n(n);
    idx_t k = __k_for_n(n, r);

    uint32_t prime_k    = get_prime((idx_t)k);
    uint32_t prime_np1  = get_prime(r + 1);

    seq_t prod = (seq_t)prime_k * (seq_t)prime_np1;

    return prime_pi((idx_t)prod);
}
