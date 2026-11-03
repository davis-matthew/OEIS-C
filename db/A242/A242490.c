/*
 * A242490  Smallest even number k such that lpf(k-3) = prime(n) while
 *          lpf(k-1) > lpf(k-3), where lpf=least prime factor (A020639).
 * https://oeis.org/A242490
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest even k such that lpf(k‑3)=prime(n) and lpf(k‑1)
 *            > lpf(k‑3)
 * Range:     u8 n <= 7, u16 n <= 50, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Validated: standard, pass, 61 terms through n = 873 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
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

/* Return the smallest prime factor of n   (n > 1) */
static inline idx_t lpf(idx_t n)
{
    if (n % 2 == 0) return 2;
    for (idx_t i = 3; i <= n / i; i += 2)
        if (n % i == 0) return i;
    return n;   /* n itself is prime – should not be called for primes */
}

/* Slow but correct implementation:  for each n≥2 search increasing odd m */
seq_t a(idx_t n)
{
    /* Undefined for n < 2; return 0 for safety */
    if (n < 2) return 0;

    /* nth prime (1‑based) */
    idx_t p = get_prime(n);   /* get_prime returns uint32_t, safe to widen to idx_t */

    /* Search odd multipliers m  (m=1,3,5,…) */
    for (idx_t m = 1; ; m += 2)
    {
        /* Ensure m has no prime factor < p (i.e., lpf(m) ≥ p)  */
        if (m == 1 || lpf(m) >= p)
        {
            /* k = p * m + 3  (k is even because p,m are odd) */
            seq_t k = ((seq_t)p) * m + 3;

            /* k‑1 must be p‑rough: its least prime factor > p */
            idx_t km1 = (idx_t)(k - 1);   /* safe for n up to 10 000 */
            if (lpf(km1) > p)
            {
                return k;
            }
        }
    }
}
