/*
 * A379493  a(n) = A276086(A001065(n)), where A276086 is the primorial base
 *          exp-function, and A001065 is the sum of proper divisors of n.
 * https://oeis.org/A379493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A276086(A001065(n))
 * Range:     u8 n <= 17, u16 n <= 79, u32 n <= 431, u64 n <= 2495,
 *            u128 n <= 30030, big any n
 * Validated: standard, pass, 88 terms through n = 8582 (2026-11-01)
 * Helpers:   ipow_idx, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A276086 is the primorial‑base exponent function and
   A001065 is the sum of proper divisors of n.  The function
   computes the primorial base representation of the sum
   of proper divisors and multiplies the corresponding primes
   raised to the obtained digit values. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/ipow_idx.h"
#include "../utils/next_prime.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* compute the sum of proper divisors: sigma(n) – n */
    seq_t s = sigma_of(n) - (seq_t)n;
    if (s == (seq_t)0)          /* A276086(0) = 1 by definition */
        return (seq_t)1;

    /* pre‑allocate for up to ~120 primes (s is at most a few 1e6) */
    static uint64_t primes[128];
    static seq_t   primorials[128];

    primorials[0] = (seq_t)1;          /* primorial for position 0 */
    size_t pc = 0;                     /* number of primes used */
    seq_t prim = (seq_t)1;

    /* build primes and primorials until the next primorial would exceed s */
    while (prim <= s) {
        uint64_t p;
        if (pc == 0)
            p = 2;                    /* first prime */
        else
            p = next_prime(primes[pc - 1]);

        primes[pc] = p;
        prim = prim * (seq_t)p;      /* new primorial */
        pc++;
        primorials[pc] = prim;       /* store primorial for this position */

        if (prim > s)                /* the primorial just built is the first
                                        one > s, so we exit after storing it */
            break;
    }

    /* last valid primorial index = pc - 1 (primorials[pc] > s) */
    seq_t rem = s;
    seq_t prod = (seq_t)1;

    for (size_t k = pc - 1; k != (size_t)-1; --k) {
        seq_t cnt = rem / primorials[k];
        rem     %= primorials[k];
        if (cnt == (seq_t)0)
            continue;                 /* skip factor if exponent is zero */
        seq_t term = ipow_idx((seq_t)primes[k], (idx_t)cnt);
        prod = seq_mul(prod, term);
    }

    return prod;
}
