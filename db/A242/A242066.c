/*
 * A242066  The smallest even k such that lpf_3(k-3) > lpf_3(k-1) >= p_n,
 *          where lpf_3(n) = lpf(n/3^t) (cf. A020639) such that 3^t (t>=0) is
 *          the maximal power of 3 which divides n.
 * https://oeis.org/A242066
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest even k such that lpf_3(k-3) > lpf_3(k-1) >= p_n,
 *            where lpf_3(n) removes all factors of 3 from n and then returns
 *            the largest prime factor (lpf)
 * Range:     u8 n <= 20, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 55 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Return the smallest prime factor of n   (n > 1) */
static inline idx_t lpf(idx_t n)
{
    if (n % 2 == 0) return 2;
    for (idx_t i = 3; i <= n / i; i += 2)
        if (n % i == 0) return i;
    return n;   /* n itself is prime – should not be called for primes */
}

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

static inline idx_t strip_three(idx_t x)
{
    while (x % 3 == 0)
        x /= 3;
    return x;
}

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;   /* undefined below offset */

    idx_t pn = get_prime(n);      /* p_n */
    for (idx_t k = 4; ; k += 2)   /* k must be even */
    {
        idx_t lpf1 = lpf(strip_three(k - 1));   /* lpf_3(k-1) */
        idx_t lpf2 = lpf(strip_three(k - 3));   /* lpf_3(k-3) */
        if (lpf1 >= pn && lpf2 > lpf1)
            return (seq_t)k;
    }
}
