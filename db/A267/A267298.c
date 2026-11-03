/*
 * A267298  Integers k such that the k-th prime divides the product of the
 *          first k nonzero Jacobsthal numbers.
 * https://oeis.org/A267298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the nth integer k for which the k‑th prime p_k divides
 * some non‑zero Jacobsthal number J_j with 1 ≤ j ≤ k.  This is equivalent to
 * the condition that the multiplicative order of 2 modulo p_k, denoted ord,
 * satisfies the inequality jmin ≤ k where
 *     jmin = 2*ord               if ord is odd,
 *     jmin = ord/2               if ord even and (ord/2) odd,
 *     jmin = ord                 otherwise. */

/* Required utilities */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

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

/* modular exponentiation: (base^exp) mod mod */
static uint32_t mod_pow_uint(uint32_t base, uint32_t exp, uint32_t mod)
{
    uint64_t result = 1;
    uint64_t b = base % mod;
    while (exp)
    {
        if (exp & 1U)
            result = (result * b) % mod;
        b = (b * b) % mod;
        exp >>= 1U;
    }
    return (uint32_t)result;
}

/* multiplicative order of 2 modulo prime p (p > 2) */
static uint32_t order_of_2(uint32_t p)
{
    uint32_t n = p - 1U;
    uint32_t ord = n;
    uint32_t temp = n;
    for (uint32_t q = 2U; (uint64_t)q * q <= temp; ++q)
    {
        if (temp % q == 0U)
        {
            while (temp % q == 0U)
                temp /= q;
            while (ord % q == 0U && mod_pow_uint(2U, ord / q, p) == 1U)
                ord /= q;
        }
    }
    if (temp > 1U)
    {
        uint32_t q = temp;
        while (ord % q == 0U && mod_pow_uint(2U, ord / q, p) == 1U)
            ord /= q;
    }
    return ord;
}

/* Cached results of the sequence */
static idx_t *seq_cache   = NULL;   /* cached k values */
static size_t  cache_len  = 0;      /* number of values stored */
static size_t  cache_cap  = 0;      /* allocated capacity */
static idx_t   next_k     = 1;      /* next k to test */

seq_t a(idx_t n)
{
    /* Generate terms until we have at least n values */
    while (cache_len < (size_t)n)
    {
        idx_t k = next_k;
        uint32_t p = get_prime(k);    /* k‑th prime */
        bool ok = false;

        /* Primes 2 and 3 never divide any Jacobsthal number,
         * so we skip them automatically. */
        if (p > 3U)
        {
            uint32_t ord = order_of_2(p);   /* ord ≥ 1 */

            /* Compute the minimal j such that p | J_j */
            idx_t jmin;
            if (ord & 1U)               /* ord odd */
                jmin = 2ULL * ord;
            else                        /* ord even */
            {
                idx_t half = ord / 2U;
                jmin = (half & 1U) ? half : (idx_t)ord;
            }

            if (jmin <= k)
                ok = true;
        }

        if (ok)
        {
            /* Ensure capacity for the cache */
            if (cache_len == cache_cap)
            {
                cache_cap = cache_cap ? cache_cap * 2U : 16U;
                idx_t *tmp = (idx_t *)realloc(seq_cache, cache_cap * sizeof(idx_t));
                seq_cache = tmp;               /* assumed success */
            }
            seq_cache[cache_len++] = k;
        }
        ++next_k;
    }
    return (seq_t)seq_cache[(size_t)n - 1U];
}
