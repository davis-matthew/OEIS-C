/*
 * A268310  Largest k > 0 such that a base b with 1 < b < c exists such that
 *          b^(c-1) == 1 (mod c^k), where c is the n-th composite number, or 0
 *          if no such k exists.
 * https://oeis.org/A268310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A268310
   Formula: a(n) = largest k>0 such that ∃b (1<b<c) with
           b^(c-1) ≡ 1 (mod c^k), where c = n‑th composite (c>1).
   If no such b and k, a(n)=0.
   The implementation factors each composite, tests all units
   b modulo c for the congruence mod c, and then lifts the modulus
   to find the maximal k.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include <inttypes.h>

#include "../utils/mod_pow.h"   /* seq_t mod_pow(seq_t, uint64_t, seq_t) */
#include "../utils/pow_uint.h"  /* seq_t pow_uint(uint64_t, idx_t) */

static uint32_t *composites = NULL;   /* list of composite numbers (c) */
static size_t      composites_cnt  = 0;   /* number of composites stored   */
static size_t      composites_cap  = 0;   /* allocated capacity            */
static size_t      composites_last = 3;   /* last checked number in sieve  */

static uint32_t *spf = NULL;          /* smallest prime factor array   */
static size_t    spf_cap = 0;
static bool     *is_prime = NULL;      /* sieve array                   */
static size_t    is_prime_cap = 0;

/*------------------------------------------------------------------*/
/* Sieve to generate composites up to at least `need` composites  */
/*------------------------------------------------------------------*/
static void _ensure_composites(size_t need)
{
    if (composites_cnt >= need)
        return;

    size_t limit = composites_last + 1;
    while (composites_cnt < need) {
        /* double the range each iteration */
        limit = limit ? limit * 2 + 1 : 500;

        /* Resize sieve arrays if necessary */
        if (limit + 1 > is_prime_cap) {
            is_prime = (bool *)realloc(is_prime, (limit + 1) * sizeof(bool));
            if (!is_prime) abort();
            spf = (uint32_t *)realloc(spf, (limit + 1) * sizeof(uint32_t));
            if (!spf) abort();
            is_prime_cap = limit + 1;
            spf_cap = limit + 1;
        }

        /* init */
        memset(is_prime, 1, (limit + 1) * sizeof(bool));
        memset(spf, 0, (limit + 1) * sizeof(uint32_t));
        is_prime[0] = is_prime[1] = false;

        /* sieve */
        for (size_t i = 2; i * i <= limit; ++i) {
            if (is_prime[i]) {
                for (size_t j = i * i; j <= limit; j += i) {
                    if (is_prime[j]) is_prime[j] = false;
                    if (!spf[j]) spf[j] = (uint32_t)i;
                }
            }
        }
        /* mark primes' SPF */
        for (size_t i = 2; i <= limit; ++i)
            if (is_prime[i] && !spf[i]) spf[i] = (uint32_t)i;

        /* collect composites */
        for (size_t i = composites_last + 1; i <= limit; ++i) {
            if (!is_prime[i]) {
                if (composites_cnt == composites_cap) {
                    composites_cap = composites_cap ? composites_cap * 2 : 64;
                    composites = (uint32_t *)realloc(composites,
                                      composites_cap * sizeof(uint32_t));
                    if (!composites) abort();
                }
                composites[composites_cnt++] = (uint32_t)i;
            }
        }
        composites_last = limit;
    }
}

/*------------------------------------------------------------------*/
/* Return the n-th composite number (1‑based)                      */
/*------------------------------------------------------------------*/
static uint32_t nth_composite(idx_t n)
{
    _ensure_composites((size_t)n);
    return composites[n - 1];
}

/*------------------------------------------------------------------*/
/* Euclidean GCD for uint64_t                                       */
/*------------------------------------------------------------------*/
static inline uint64_t _gcd_u64(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

/*------------------------------------------------------------------*/
/* Factor c into prime powers. Scalars allocated on the stack.     */
/*------------------------------------------------------------------*/
static size_t _factor_uint32(uint32_t n, uint32_t *pf, uint32_t *pe)
{
    size_t cnt = 0;
    while (n > 1) {
        uint32_t p = spf[n];
        uint32_t e = 0;
        while (n % p == 0) {
            n /= p;
            e++;
        }
        pf[cnt] = p;
        pe[cnt] = e;
        cnt++;
    }
    return cnt;
}

/*------------------------------------------------------------------*/
/* Compute the maximum k for a single base b modulo c.               */
/*------------------------------------------------------------------*/
static seq_t _compute_k(uint32_t c, uint32_t b)
{
    seq_t max_k = 1;
    seq_t cur_mod = (seq_t)c;             /* c^1 */

    /* The exponent m = c-1 is small (≤ ~120k), so use mod_pow directly. */
    while (1) {
        /* Compute next modulus = c^(k+1) */
        seq_t next_mod;
        if (sizeof(seq_t) == sizeof(uint64_t)) {
            uint64_t uc = (uint64_t)cur_mod;
            if (uc > UINT64_MAX / c) break;   /* would overflow the 64‑bit type */
            next_mod = (seq_t)(uc * (uint64_t)c);
        } else {
            /* For larger integer types, use pow_uint (exponent is small) */
            next_mod = pow_uint((uint64_t)c, (idx_t)(max_k + 1));
        }

        if (mod_pow((seq_t)b, (uint64_t)(c - 1), next_mod) != (seq_t)1)
            break;

        max_k = max_k + 1;
        cur_mod = next_mod;
    }

    return max_k;
}

/*------------------------------------------------------------------*/
/* Main sequence function                                          */
/*------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    uint32_t c = nth_composite(n);

    /* Factor c once for possible speedups (currently unused, but keeps API). */
    uint32_t factors[32], exponents[32];
    size_t nf = _factor_uint32(c, factors, exponents);

    seq_t overall_max_k = (seq_t)0;

    /* Iterate all units modulo c */
    for (uint32_t b = 2; b < c; ++b) {
        if (_gcd_u64((uint64_t)b, (uint64_t)c) != 1) continue;

        if (mod_pow((seq_t)b, (uint64_t)(c - 1), (seq_t)c) != (seq_t)1)
            continue;

        seq_t k = _compute_k(c, b);
        if (k > overall_max_k) overall_max_k = k;
    }

    return overall_max_k;
}
