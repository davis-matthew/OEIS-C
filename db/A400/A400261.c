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
 * Helpers:   factor_u64, msdigit_primorial_base
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/msdigit_primorial_base.h"

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
