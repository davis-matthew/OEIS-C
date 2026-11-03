/*
 * A245458  a(n) = ((prime(n)-2)!+2) mod prime(n)# (cf. A002110).
 * https://oeis.org/A245458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 100 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A245458
 *  Formula: a(n) = ((prime(n)-2)! + 2) mod prime(n)#,
 *  where prime(n)# = ∏_{k=1}^n prime(k).
 *  This implementation uses the Chinese Remainder Theorem:
 *  For n ≥ 3 the residues modulo the first n-1 primes are 2,
 *  and modulo prime(n) the residue is 3.
 *  The special cases n = 1 and n = 2 are handled directly.
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
    if (n < 1) return 0;           /* undefined for n < 1 */
    if (n == 1) return 1;          /*  (2-2)!+2 = 3  mod 2 = 1 */
    if (n == 2) return 3;          /*  (3-2)!+2 = 3  mod 6 = 3 */

    seq_t x   = 0;                 /* current solution modulo 'mod' */
    seq_t mod = 1;                 /* current modulus (product of processed primes) */

    for (idx_t i = 1; i <= n; ++i) {
        uint32_t p   = get_prime(i);          /* i‑th prime (fits in 32 bit) */
        uint64_t r64 = (i == n) ? 3 : 2;      /* residue for this prime */

        seq_t p_ts = (seq_t)p;
        seq_t r_ts = (seq_t)r64;

        /* compute x modulo p */
        seq_t x_mod_p = x % p_ts;

        /* diff = (r - x) mod p, computed without unsigned wrap */
        seq_t diff;
        if (r_ts >= x_mod_p) diff = r_ts - x_mod_p;
        else                 diff = r_ts + p_ts - x_mod_p;

        /* modular inverse of current modulus modulo p
           (mod % p)^(p-2) (mod p) */
        seq_t mod_mod_p = mod % p_ts;
        seq_t inv       = powmod(mod_mod_p, (idx_t)(p - 2), p_ts);  /* Fermat's little theorem */

        /* k = diff * inv mod p */
        seq_t k = (diff * inv) % p_ts;

        /* update solution: x = x + k * mod */
        x   = x + k * mod;
        mod = mod * p_ts;   /* new modulus for next step */
    }

    return x;   /* minimal positive residue modulo product of first n primes */
}
