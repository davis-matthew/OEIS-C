/*
 * A322553  Odd numbers whose product of prime indices is a prime power.
 * https://oeis.org/A322553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A322553: odd numbers whose product of prime indices is a prime power.
   The function computes the n-th term (offset 1). */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

/* Global prime table ------------------------------------------------------- */

static uint64_t *primes = NULL;   /* array of primes, 1‑based indices    */
static size_t   primes_cnt = 0;   /* number of primes                   */

/* Initialize prime table once, up to a generous limit. */
static void init_primes(void)
{
    if (primes != NULL)
        return;

    /* 20 000 000 is enough for all primes needed up to about the 10 th 000‑th term. */
    const uint64_t SIEVE_LIMIT = 20000000ULL;
    build_primes_upto(SIEVE_LIMIT, &primes, &primes_cnt);
}

/* -------------------------------------------------------------------------- */
/* Binary search: return the 1‑based index of prime p in the global table. */
/* Returns 0 if p is not a prime in the table. */
static uint32_t prime_index(uint64_t p)
{
    size_t low = 0, high = primes_cnt;
    while (low < high) {
        size_t mid = (low + high) >> 1;
        uint64_t val = primes[mid];
        if (val == p)
            return (uint32_t)(mid + 1);   /* 1‑based index */
        if (val < p)
            low = mid + 1;
        else
            high = mid;
    }
    return 0;
}

/* -------------------------------------------------------------------------- */
/* Factor n into distinct prime factors.  The factors are written into out[0..cnt‑1]. */
static void factor_uint64(uint64_t n, uint64_t *out, size_t *cnt)
{
    size_t i = 0;
    for (size_t j = 0; j < primes_cnt && (uint64_t)primes[j] * primes[j] <= n; ++j) {
        uint64_t p = primes[j];
        if (n % p == 0) {
            out[i++] = p;
            while (n % p == 0)
                n /= p;
        }
    }
    if (n > 1)          /* remaining prime > sqrt(original n) */
        out[i++] = n;
    *cnt = i;
}

/* -------------------------------------------------------------------------- */
/* Return the unique prime base of x if x is a power of a prime, else 0. */
static uint64_t prime_power_base(uint64_t x)
{
    if (x <= 1)
        return 0;

    uint64_t q = 0;
    for (size_t j = 0; j < primes_cnt && (uint64_t)primes[j] * primes[j] <= x; ++j) {
        uint64_t p = primes[j];
        if (x % p == 0) {
            q = p;
            while (x % p == 0)
                x /= p;
            break;
        }
    }
    if (x == 1)               /* x was a pure power of found prime q */
        return q;
    if (x > 1) {               /* x is a prime > sqrt(original x) */
        /* Check that we did not find a second prime factor */
        return (q == 0) ? x : (q == x ? q : 0);
    }
    return 0;
}

/* -------------------------------------------------------------------------- */
/* Check whether an odd integer n>1 satisfies the "prime‑index power" condition. */
static bool satisfies_property(uint64_t n)
{
    uint64_t facs[64];
    size_t fac_cnt = 0;
    factor_uint64(n, facs, &fac_cnt);

    uint64_t base_prime = 0;

    for (size_t i = 0; i < fac_cnt; ++i) {
        uint64_t p   = facs[i];
        uint32_t idx = prime_index(p);
        if (idx == 0)                /* prime index not known – fail   */
            return false;

        /* indices 1 (prime 2) never occur for odd n, but handle just in case */
        if (idx == 1)
            continue;

        uint64_t q = prime_power_base(idx);
        if (q == 0)                  /* idx is not a prime power      */
            return false;

        if (base_prime == 0)
            base_prime = q;
        else if (base_prime != q)
            return false;
    }
    return base_prime != 0;
}

/* -------------------------------------------------------------------------- */
/* Public sequence function --------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* undefined for offset 1, harmless */

    if (n == 1)
        return (seq_t)1;           /* a(1) = 1 */

    init_primes();

    uint64_t count = 1;  /* count already includes term 1 */
    uint64_t odd   = 3;  /* next odd candidate */

    while (true) {
        if (satisfies_property(odd)) {
            ++count;
            if (count == (uint64_t)n)
                return (seq_t)odd;
        }
        odd += 2;               /* go to the next odd integer */
    }
}
