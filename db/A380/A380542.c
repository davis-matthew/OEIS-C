/*
 * A380542  a(n) is the multiplicative order of -9 modulo prime(n); a(2) = 0
 *          for completion.
 * https://oeis.org/A380542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = multiplicative order of -9 modulo prime(n)
 * Range:     u8 n <= 54, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9210 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)0; /* prime 3 gives -9 ≡ 0 mod 3 */

    /* nth prime */
    uint32_t p = get_prime(n);
    if (p == 3) return (seq_t)0;

    uint64_t mod = p;
    uint64_t g = (mod - (9 % mod)) % mod;   /* -9 mod p */

    uint64_t ord = mod - 1;                 /* p-1 */

    /* factorise p-1 */
    uint64_t temp = ord;
    uint32_t factors[64];
    size_t nf = 0;
    for (uint64_t i = 2; i * i <= temp; ++i) {
        if (temp % i == 0) {
            factors[nf++] = (uint32_t)i;
            while (temp % i == 0) temp /= i;
        }
    }
    if (temp > 1) {
        factors[nf++] = (uint32_t)temp;
    }

    /* reduce ord by prime factors */
    for (size_t j = 0; j < nf; ++j) {
        uint32_t q = factors[j];
        while (ord % q == 0) {
            uint64_t cand = ord / q;
            seq_t val = powmod((seq_t)g, cand, (seq_t)mod);
            if (val == (seq_t)1)
                ord = cand;
            else
                break;
        }
    }

    return (seq_t)ord;
}
