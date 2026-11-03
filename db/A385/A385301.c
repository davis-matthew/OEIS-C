/*
 * A385301  a(n) = Sum_{k=0..p-1} 1/k! mod p where p is prime(n) and 1/k! is
 *          the inverse of k! modulo p.
 * https://oeis.org/A385301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..prime(n)-1} 1/k! mod prime(n)
 * Range:     u8 n <= 54, u16 n <= 2585, u32 n <= 2585, u64 n <= 2585,
 *            u128 n <= 2585, big any n
 * Validated: standard, pass, 86 terms through n = 2338 (2026-11-03)
 * Helpers:   mem, mulmod, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include "../utils/mulmod.h"
#include "../utils/powmod.h"
#include "../utils/mem.h"

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
    /* get the n-th prime */
    uint64_t p = get_prime(n);
    if (p == 0) return 0;          /* safety guard */

    seq_t mod = (seq_t)p;          /* modulo value */
    size_t len = (size_t)p;        /* number of terms in sum (p) */

    /* Allocate array for factorials modulo p */
    seq_t *fact = mem_new(seq_t, len);
    if (!fact) return 0;           /* allocation failure */

    /* Compute factorials modulo p */
    fact[0] = 1 % mod;
    for (size_t k = 1; k < len; ++k) {
        fact[k] = (seq_t)mulmod((uint64_t)fact[k - 1], (uint64_t)k,
                                (uint64_t)mod);
    }

    /* Compute inverse factorial of (p-1)! via Fermat */
    seq_t inv = (seq_t)powmod(fact[len - 1], p - 2, mod);  /* inv[p-1] */

    /* Accumulate the sum of inverse factorials */
    uint64_t acc = 0;
    acc += (uint64_t)inv;
    if (acc >= (uint64_t)mod) acc -= (uint64_t)mod;

    /* Backward loop: inv[k-1] = inv[k] * k (mod p) */
    for (size_t k = len - 1; k > 0; --k) {
        inv = (seq_t)mulmod((uint64_t)inv, (uint64_t)k,
                             (uint64_t)mod);   /* inv[k-1] */
        acc += (uint64_t)inv;
        if (acc >= (uint64_t)mod) acc -= (uint64_t)mod;
    }

    mem_free(fact);
    return (seq_t)(acc % (uint64_t)mod);
}
