/*
 * A380165  a(n) is the value of the Euler totient function when applied to
 *          the largest unitary divisor of n that is an exponentially odd
 *          number.
 * https://oeis.org/A380165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = φ(Π_{p^e‖n, e odd} p^e) (multiplicative: a(p^e) = φ(p^e)
 *            if e is odd, otherwise 1)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 9942 (2026-11-01)
 * Helpers:   factor_u64, ipow_i32, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_i32.h"
#include "../utils/totient_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;          /* defined only for n≥1, but guard anyway */

    /* Factor n into primes and exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;   /* product over odd‑exponent primes */

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t e = exps[i];
        if ((e & 1ULL) == 0)      /* skip even exponents */
            continue;

        /* φ(p^e) = (p-1) * p^(e-1) */
        seq_t p = (seq_t)primes[i];
        seq_t pow_p = ipow_i32(p, (int)(e - 1));
        seq_t phi_local = (p - 1) * pow_p;

        result *= phi_local;
    }

    return result;
}
