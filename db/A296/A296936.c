/*
 * A296936  Inert rational primes in the field Q(sqrt(11)).
 * https://oeis.org/A296936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 3306, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9922 (2026-11-03)
 * Helpers:   is_prime, mem, powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Inert rational primes in Q(sqrt(11)):
 * primes p (p ≠ 2,11) for which 11 is a quadratic non‑residue modulo p,
 * i.e. (11/p) = -1.  The sequence is the list of these primes in
 * increasing order.  The function a(n) returns the n‑th inert prime
 * (with a(1)=3).  It works for any build that supplies seq_t and
 * idx_t, using the provided primality test and modular exponentiation.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/powmod_u64.h"
#include "../utils/mem.h"

/* Static storage for the inert primes */
static seq_t *inert_primes = NULL;   /* array of inert primes */
static size_t inert_cap   = 0;       /* allocated capacity (in seq_t units) */
static idx_t  inert_cnt   = 0;       /* number stored */

/* Next integer to test for primality (after 2) */
static idx_t next_candidate = 3;

/* Ensure that at least 'need' inert primes are available. */
static void ensure_inert(idx_t need)
{
    /* Dynamically grow the array if required */
    if (inert_cap < need) {
        size_t new_cap = inert_cap ? inert_cap * 2 : 64;
        if (new_cap < (size_t)need)
            new_cap = (size_t)need;
        seq_t *tmp = mem_resize(seq_t, inert_primes, new_cap);
        if (!tmp) abort();          /* out of memory */
        inert_primes = tmp;
        inert_cap = new_cap;
    }

    while (inert_cnt < need) {
        /* Test if current candidate is prime */
        if (is_prime(next_candidate)) {
            /* Skip ramified primes */
            if (next_candidate != 11) {
                /* Legendre symbol (11 / next_candidate) via Euler's criterion */
                uint64_t p   = (uint64_t)next_candidate;
                uint64_t leg = powmod_u64(11, (p - 1) / 2, p);
                if (leg == p - 1) { /* Legendre = -1 => inert prime */
                    inert_primes[inert_cnt++] = (seq_t)next_candidate;
                }
            }
        }
        next_candidate++;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety for n < 1 */
    ensure_inert(n);                     /* ensure inert_primes[n-1] exists */
    return inert_primes[n - 1];
}
