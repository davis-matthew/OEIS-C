/*
 * A353214  a(n) = 2^A007013(4) mod prime(n); the last term of this sequences
 *          is when a(n) = 1.
 * https://oeis.org/A353214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, fini
 * Formula:   a(n) = 2^(2^127 − 1) mod prime(n), expressed in the centered
 *            residue system {−⌊p/2⌋, …, ⌊p/2⌋}
 * Range:     i8 n <= 31, i16 n <= 61, i32 n <= 61, i64 n <= 61, i128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-01)
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

/* compute a(n) for n ≥ 1, using 1‑based indexing as in OEIS */
seq_t a(idx_t n)
{
    /* Retrieve the n‑th prime (prime(1) = 2). */
    seq_t p = (seq_t)get_prime(n);

    /* Special case: prime 2 is the only even prime, 2^k mod 2 = 0 for k>0 */
    if (p == 2)
        return (seq_t)0;            /* 0 is already the centered residue */

    /* φ(p) = p−1 (Euler's totient for a prime) */
    seq_t phi = p - 1;

    /* Compute 2^127 mod φ(p). 127 fits in uint64_t. */
    seq_t pow2_127 = powmod((seq_t)2, (uint64_t)127, phi);

    /* (2^127−1) mod φ(p) = (pow2_127 − 1) mod φ(p) */
    seq_t exp_mod = (pow2_127 + phi - 1) % phi;

    /* Compute 2^(2^127−1) mod prime(n) using the reduced exponent. */
    seq_t r = powmod((seq_t)2, (uint64_t)exp_mod, p);

    /* Convert to centered residue in range −⌊p/2⌋ … ⌊p/2⌋. */
    seq_t half = p / 2;               /* floor(p/2) */
    if (r > half)
        r -= p;

    return r;
}
