/*
 * A243768  Primes p such that 6 is not a 6th power mod p.
 * https://oeis.org/A243768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 4385, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9989 (2026-11-03)
 * Helpers:   gcd_u64, is_prime, mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th prime p for which 6 is not a 6‑th power modulo p.
 *
 * The definition is checked by searching primes in increasing order.
 * For each candidate prime p, the condition is evaluated by
 *   let d = gcd(6,p-1),  e = (p-1)/d
 *   compute r = 6^e (mod p)
 *   if r != 1 then p qualifies.
 *
 * The function caches found primes to answer arbitrary queries in
 * constant time after the initial search phase.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include <string.h>
#include "../utils/gcd_u64.h"
#include "../utils/is_prime.h"
#include "../utils/powmod.h"
#include "../utils/mem.h"

seq_t a(idx_t n) {
    /* Static cache of qualifying primes. */
    static seq_t *qual = NULL;
    static size_t qcnt = 0;
    static size_t qcap = 0;

    /* Candidate prime generator.  Start at the first qualifying prime. */
    static uint64_t cand = 7;   /* lowest prime satisfying condition */

    /* Expand cache until we have at least n qualifying primes. */
    while (qcnt < n) {
        /* Skip to the next odd number if needed. */
        while (!is_prime(cand))
            cand += 2;       /* is_prime handles even numbers as well */

        uint64_t p = cand;

        /* Determine if 6 is a 6th power mod p. */
        uint64_t d = gcd_u64(6ULL, p - 1);
        uint64_t exp = (p - 1) / d;
        seq_t r = powmod((seq_t)6, exp, (seq_t)p);

        /* Store if 6 is not a 6th power residue. */
        if (r != (seq_t)1) {
            if (qcap == 0) {
                qcap = 16;
                qual = mem_new(seq_t, qcap);
            } else if (qcnt >= qcap) {
                qcap <<= 1;
                qual = mem_resize(seq_t, qual, qcap);
            }
            qual[qcnt++] = (seq_t)p;
        }

        /* Advance to next odd candidate. */
        cand += 2;
    }

    return qual[n - 1];
}
