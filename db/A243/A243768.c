/*
 * A243768  Primes p such that 6 is not a 6th power mod p.
 * https://oeis.org/A243768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 4385, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd64, is_prime, mod_pow
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

#include "../utils/is_prime.h"   /* bool is_prime(idx_t n); */
#include "../utils/mod_pow.h"    /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */
#include "../utils/gcd64.h"

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
        uint64_t d = gcd64(6ULL, p - 1);
        uint64_t exp = (p - 1) / d;
        seq_t r = mod_pow((seq_t)6, exp, (seq_t)p);

        /* Store if 6 is not a 6th power residue. */
        if (r != (seq_t)1) {
            if (qcap == 0) {
                qcap = 16;
                qual = (seq_t*)malloc(qcap * sizeof(seq_t));
            } else if (qcnt >= qcap) {
                qcap <<= 1;
                qual = (seq_t*)realloc(qual, qcap * sizeof(seq_t));
            }
            qual[qcnt++] = (seq_t)p;
        }

        /* Advance to next odd candidate. */
        cand += 2;
    }

    return qual[n - 1];
}
