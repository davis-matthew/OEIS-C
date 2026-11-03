/*
 * A282445  For n>=5, a(n) is the smallest m>=3 such that odd part of
 *          ((prime(n)^2 - prime(m)^2)/3) is prime, or a(n)=0 if there is no
 *          such m<n.
 * https://oeis.org/A282445
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest m ≥ 3 such that the odd part of
 *            ((prime(n)^2 – prime(m)^2)/3) is prime; a(n)=0 if no such m<n
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 77 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, odd_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/odd_part.h"

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

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

seq_t a(idx_t n)
{
    if (n < 5U)
        return 0;              /* undefined for n < 5, return 0 for safety */

    seq_t p_n = (seq_t)get_prime(n);   /* nth prime */

    for (idx_t m = 3U; m < n; ++m)     /* search m from 3 up to n-1 */
    {
        seq_t p_m = (seq_t)get_prime(m);
        seq_t diff = p_n * p_n - p_m * p_m;   /* (p_n² – p_m²) */
        seq_t val  = diff / 3;                /* divide by 3 */
        seq_t odd  = odd_part(val);           /* odd part of val */

        if (isPrimeGeneric(odd))
            return (seq_t)m;                  /* first such m */
    }

    return 0;   /* no suitable m found */
}
