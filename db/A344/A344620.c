/*
 * A344620  Primes p such that there is no positive integer a with 2*a, a^2-1
 *          and a^2+1 not only smaller than p but also quadratic residues
 *          modulo p.
 * https://oeis.org/A344620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A344620: primes p such that there is no positive integer a with
   2*a, a^2-1 and a^2+1 all < p and all quadratic residues modulo p.
   The sequence offset is 1; a(1) = 2.  */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

static seq_t *cache = NULL;          /* array of found primes */
static size_t cache_cap = 0, cache_count = 0;
static idx_t next_prime = 2;         /* next candidate prime to test */

/* Ensure the cache can hold at least 'need' elements. */
static void
ensure_capacity(size_t need)
{
    if (need > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while (new_cap < need) new_cap *= 2;
        seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_arr) abort();
        cache = new_arr;
        cache_cap = new_cap;
    }
}

/* Return true iff the prime p admits a positive integer a with
   2*a, a^2-1 and a^2+1 all < p and simultaneously quadratic residues
   modulo p.  For such a prime the condition fails (i.e. the prime
   should NOT be in the sequence). */
static bool
prime_has_valid_a(idx_t p)
{
    /* a_max = floor(sqrt(p-2)) – largest a with a^2+1 < p   */
    idx_t a_max = (idx_t)floor(sqrt((long double)p - 2.0L));
    uint64_t half = (p - 1) / 2;   /* (p-1)/2, used as exponent */

    for (idx_t a = 1; a <= a_max; ++a) {
        seq_t r1 = ((seq_t)2 * a) % p;          /* 2*a  ( < p )          */
        if (r1 == (seq_t)0) continue;          /* ignore zero residue   */
        seq_t aa = (seq_t)a * a;
        seq_t r2 = (aa - 1) % p;                /* a^2-1                  */
        if (r2 == (seq_t)0) continue;
        seq_t r3 = (aa + 1) % p;                /* a^2+1                  */
        if (r3 == (seq_t)0) continue;

        if (mod_pow(r1, half, p) == (seq_t)1 &&
            mod_pow(r2, half, p) == (seq_t)1 &&
            mod_pow(r3, half, p) == (seq_t)1) {
            /* Found a counter‑example; this prime is excluded. */
            return true;
        }
    }
    /* No such a was found – the prime belongs to the sequence. */
    return false;
}

seq_t
a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* undefined for n < 1 */

    while (cache_count < n) {
        /* Find the next prime. */
        while (!is_prime(next_prime)) ++next_prime;

        idx_t p = next_prime;
        ++next_prime;               /* prepare for next search */

        if (!prime_has_valid_a(p)) {
            ensure_capacity(cache_count + 1);
            cache[cache_count++] = (seq_t)p;
        }
    }
    return cache[n - 1];
}
