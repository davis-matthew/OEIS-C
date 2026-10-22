/*
 * A332455  Starting from sigma(n)+1, number of tripling steps to reach 1 in
 *          '3x+1' problem, or -1 if 1 is never reached.
 * https://oeis.org/A332455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 6355, u32 n <= 9795, u64 n <= 9795,
 *            u128 n <= 9795, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A332455: a(n) = number of odd steps in the Collatz sequence
   starting from 1 + σ(n), where σ(n) is the sum of divisors of n.
   If 1 is not reached, a(n) would be −1 (not needed for tested
   input values).  Offset: n = 1. */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* --- compute σ(n) --- */
    seq_t sigma = 1;                 /* product of divisor sums */
    uint64_t tmp   = (uint64_t)n;    /* use 64‑bit for trial division */

    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            seq_t term  = 1;         /* 1 + p + p² + … + p^e */
            seq_t power = 1;
            while (tmp % p == 0) {
                tmp   /= p;
                power *= (seq_t)p;
                term  += power;
            }
            sigma *= term;
        }
    }
    if (tmp > 1) { /* remaining prime factor */
        sigma *= (seq_t)(1 + tmp);    /* 1 + tmp */
    }

    /* --- Collatz odd‑step count starting from 1 + σ(n) --- */
    seq_t x      = sigma + (seq_t)1;  /* starting value */
    seq_t steps  = 0;

    while (x != 1) {
        if (x & 1) {                  /* odd → 3x+1, record a step */
            steps++;
            x = 3 * x + 1;
        } else {
            x >>= 1;                   /* even → x/2, no step */
        }
    }

    return steps;
}
