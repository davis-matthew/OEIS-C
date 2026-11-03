/*
 * A241492  a(n) = |{0 < g < prime(n): g is a primitive root modulo prime(n)
 *          and g is a product of two consecutive integers}|.
 * https://oeis.org/A241492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of primitive roots g modulo prime(n) with * g
 *            = k*(k+1) for some integer k, 0 < g < prime(n). The function *
 *            evaluates this definition directly by brute force, which is fast
 *            * enough for n ≤ 10⁴. Primitive_root test uses the criterion
 *            that * g^((p−1)/q) ≠ 1 (mod p) for every di...
 * Range:     u8 n <= 54, u16 n <= 6543, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 8633 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include "../utils/powmod.h"

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

/* Helper: collect distinct prime factors of n into array `f` (max 64). */
static void unique_prime_factors(uint64_t n, uint64_t *f, size_t *cnt)
{
    size_t i = 0;
    if (n % 2ULL == 0ULL) {
        f[i++] = 2ULL;
        while (n % 2ULL == 0ULL) n /= 2ULL;
    }
    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL) {
        if (n % d == 0ULL) {
            f[i++] = d;
            while (n % d == 0ULL) n /= d;
        }
    }
    if (n > 1ULL) f[i++] = n;
    *cnt = i;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* prime(n) fits in 32-bit for n ≤ 10000, but use seq_t for modulus */
    uint32_t pr = get_prime(n);
    uint64_t p = (uint64_t)pr;          /* full prime as 64‑bit */
    seq_t p_mod = (seq_t)p;             /* for mod_pow operations */

    /* factor p-1 */
    uint64_t factors[64];
    size_t fac_cnt;
    unique_prime_factors(p - 1, factors, &fac_cnt);

    seq_t count = 0;

    /* iterate k such that k*(k+1) < p */
    for (idx_t k = 1; ; ++k) {
        unsigned __int128 prod = (unsigned __int128)k * (unsigned __int128)(k + 1);
        if (prod >= p) break;           /* no larger product will work */

        /* check if prod is a primitive root mod p */
        bool primitive = true;
        for (size_t i = 0; i < fac_cnt && primitive; ++i) {
            uint64_t exp = (p - 1) / factors[i];
            seq_t res = powmod((seq_t)(prod % p), exp, p_mod);
            if (res == 1) primitive = false;
        }
        if (primitive) ++count;
    }

    return count;
}
