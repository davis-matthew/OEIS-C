/*
 * A329229  Numbers that are the product of two odd prime powers with Euler
 *          phi-functions having solely a single 2 as a common prime factor.
 * https://oeis.org/A329229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer > 1 that is a product p^j * q^k of two
 *            distinct odd primes p and q (j, k ≥ 1) with gcd( phi(p^j),
 *            phi(q^k) ) = 2
 * Range:     u8 n <= 46, u16 n <= 8868, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/factor_u64.h"
#include "../utils/pow_seq.h"
#include "../utils/gcd.h"

/* Test whether m is a cyclic semiprime of the required form. */
static bool is_cyclic_semiprime(uint64_t m)
{
    if ((m & 1U) == 0)            /* must be odd */
        return false;

    /* factor_u64 returns distinct prime factors and
       their exponents via the provided arrays.  */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(m, primes, exps, &cnt);
    if (cnt != 2)                 /* exactly two distinct primes */
        return false;

    /* Ensure both primes are odd. */
    if (primes[0] == 2 || primes[1] == 2)
        return false;

    /* phi(p^e) = p^(e-1) * (p-1)  */
    seq_t phi1 = pow_seq((seq_t)primes[0], (int)(exps[0] - 1))
                 * ((seq_t)primes[0] - 1U);
    seq_t phi2 = pow_seq((seq_t)primes[1], (int)(exps[1] - 1))
                 * ((seq_t)primes[1] - 1U);

    seq_t g = gcd(phi1, phi2);
    return g == (seq_t)2U;
}

/* ----------  Cache for sequence values  ---------- */
static seq_t *seq_cache   = NULL;   /* array of terms */
static idx_t  cache_size  = 0;      /* how many terms are stored */
static idx_t  cache_cap   = 0;      /* allocated capacity */
static uint64_t current_val = 1;    /* next integer to test */

/* Grow the cache array if required. */
static void ensure_cache(idx_t n)
{
    while (cache_size < n)
    {
        /* progress to the next odd integer */
        do
            ++current_val;
        while ((current_val & 1U) == 0);

        if (!is_cyclic_semiprime(current_val))
            continue;

        /* allocate space if needed */
        if (cache_size == cache_cap)
        {
            cache_cap = cache_cap ? cache_cap * 2 : 16;
            seq_cache = (seq_t *)realloc(seq_cache,
                                          cache_cap * sizeof(seq_t));
        }
        seq_cache[cache_size++] = (seq_t)current_val;
    }
}

/* ----------  Public sequence function  ---------- */
extern seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n = 0 */
    ensure_cache(n);
    return seq_cache[n - 1];
}
