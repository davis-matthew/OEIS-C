/*
 * A363991  a(n) = Sum_{d|n, d odd} d^(d+1).
 * https://oeis.org/A363991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n, d odd} d^(d+1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 22,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation factors n, enumerates all odd divisors, and
   sums d^(d+1) using the provided big_pow routine.  It handles the
   arbitrary‑precision case by allocating memory with explicit casts
   that satisfy both C11 and C++17.  */ 

#include <stdlib.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* A trivial guard for the non‑positive case; the OEIS sequence
       is defined only for n >= 1.  */
    if (n == 0)
        return (seq_t)0;

    /* Convert the index to the native unsigned integer type used by
       the helper routines.  */
    uint64_t nn = (uint64_t)n;

    /* Factor n: primes[] holds the base primes, exps[] their powers,
       and pcnt the number of distinct prime factors.  */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t pcnt = 0;
    factor_u64(nn, primes, exps, &pcnt);

    /* Start with a single divisor: 1.  Allocate space for the divisor
       list; the size is expanded later.  */
    size_t divcnt = 1;
    seq_t *divs = (seq_t*)malloc(sizeof(seq_t));
    if (!divs) return (seq_t)0;          /* unlikely for n <= 10000 */
    divs[0] = (seq_t)1;

    /* For each odd prime factor multiply the current list of divisors
       by powers of that prime, thereby generating the new set of
       odd divisors.  */
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        if (p == 2) continue;                /* skip the factor 2 */

        uint64_t e = exps[i];
        size_t oldcnt = divcnt;
        size_t newcnt = oldcnt * (e + 1);

        seq_t *newdivs = (seq_t*)realloc(divs, newcnt * sizeof(seq_t));
        if (!newdivs) {
            free(divs);
            return (seq_t)0;
        }
        divs = newdivs;
        divcnt = newcnt;

        seq_t powp = (seq_t)1;               /* p^0 */
        size_t idx = 0;
        for (uint64_t exp = 0; exp <= e; ++exp) {
            for (size_t j = 0; j < oldcnt; ++j)
                divs[idx++] = divs[j] * powp;
            powp *= (seq_t)p;
        }
    }

    /* Sum d^(d+1) over the generated odd divisors.  */
    seq_t sum = (seq_t)0;
    for (size_t i = 0; i < divcnt; ++i) {
        seq_t d   = divs[i];
        seq_t exp = d + (seq_t)1;
        sum += big_pow(d, exp);
    }

    free(divs);
    return sum;
}
