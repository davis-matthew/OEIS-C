/*
 * A246763  Catalan(n)^2 mod prime(n).
 * https://oeis.org/A246763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Catalan(n)^2 mod prime(n) (prime(n) is the n-th prime)
 * Range:     u8 n <= 54, u16 n <= 5205, u32 n <= 7762, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9354 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Catalan numbers are computed modulo prime(n) using the identity  
 *   Catalan(n) = ∏_{i=2}^{n} (n+i) * inv(i)  (mod prime(n))
 * where inv(i) = i^{prime(n)-2} (mod prime(n)) by Fermat's little theorem. 
 * The result is then squared modulo the same prime. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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

seq_t a(idx_t n)
{
    /* obtain the n-th prime (fits in 32 bits for n up to 10000) */
    uint64_t p64 = get_prime(n);
    seq_t mod = (seq_t)p64;

    /* Catalan number modulo mod */
    seq_t catalan = 1;
    seq_t nn = (seq_t)n;

    for (idx_t i = 2; i <= n; ++i) {
        /* multiply by (n + i) modulo mod */
        catalan = (catalan * (nn + (seq_t)i)) % mod;

        /* multiply by the inverse of i modulo mod */
        seq_t inv = powmod((seq_t)i, p64 - 2, mod);
        catalan = (catalan * inv) % mod;
    }

    /* return Catalan(n)^2 mod prime(n) */
    return (catalan * catalan) % mod;
}
