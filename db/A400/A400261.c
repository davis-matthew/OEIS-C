/*
 * A400261  The most significant digit of n' [the arithmetic derivative of n,
 *          A003415] in the primorial base, A049345.
 * https://oeis.org/A400261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = A276153(A003415(n)) – the most significant digit of the
 *            arithmetic derivative of n expressed in primorial base.
 *            Arithmetic derivative: d(p^e) = e·p^(e-1), d(ab)=a·d(b)+b·d(a).
 *            Primorial base: digits are computed using primorials
 *            1,2,6,30,210,2310,30030,… . Implementation uses the helper fa...
 * Range:     u8 n <= 95, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 122 terms through n = 9510 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

static seq_t msdigit_primorial_base(seq_t d)
{
    if (d == (seq_t)0) return (seq_t)0;

    /* first few primes – enough for all primorials that fit in seq_t */
    const uint64_t first_primes[] =
        {2U,3U,5U,7U,11U,13U,17U,19U,23U,29U,31U,37U,
         41U,43U,47U,53U,59U,61U,67U,71U,73U,79U,83U,89U,97U};
    const size_t prime_cnt = sizeof(first_primes)/sizeof(first_primes[0]);

    seq_t primorial = (seq_t)1;   /* 1 = first primorial 0th */

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = first_primes[i];
        /* avoid overflow and stop once primorial * p would exceed d */
        if (primorial > d / p) break;
        primorial = primorial * (seq_t)p;
    }
    return d / primorial;
}

static seq_t arithmetic_derivative_u64(uint64_t n)
{
    if (n <= 1) return (seq_t)0;

    /* temporary storage for factorisation */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64(n, primes, exps, &cnt);

    seq_t base = (seq_t)n;      /* cast index to seq_t once */
    seq_t deriv = (seq_t)0;

    for (size_t i = 0; i < cnt; ++i) {
        /* term = e * (n / p)  -- all integers */
        deriv += base / (seq_t)primes[i] * (seq_t)exps[i];
    }
    return deriv;
}

seq_t a(idx_t n)
{
    /* n is unsigned 64‑bit; cast once to uint64_t */
    uint64_t index = (uint64_t)n;
    seq_t deriv = arithmetic_derivative_u64(index);
    return msdigit_primorial_base(deriv);
}
