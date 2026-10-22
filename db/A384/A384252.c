/*
 * A384252  The number of integers k from 1 to n such that the greatest
 *          divisor of k that is an infinitary divisor of n is a power of 2.
 * https://oeis.org/A384252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Π_{p^e || n} g(p, e) where g(2, e) = 2^e g(p, e) = p^e -
 *            p^{e - f} f = e if e is even, else f = 1 (equivalently, for odd
 *            primes: a(p^e) = p^e - p^(e-1) if e odd, and a(p^e) = p^e - 1 if
 *            e even)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)1; /* a(1) = 1 */
    }

    /* factor n into primes and exponents */
    size_t cnt = 0;
    /* enough for factorization of 64‑bit numbers */
    uint64_t pr[64], ex[64];
    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];
        if (p == 2ull) {
            /* a(2^e) = 2^e */
            result *= pow_uint(2ull, e);
        } else {
            uint64_t f = (e % 2 == 0 ? e : 1ull);          /* f = e if e even, else 1 */
            seq_t pe  = pow_uint(p, e);                    /* p^e */
            seq_t pe_f = pow_uint(p, e - f);               /* p^{e-f} */
            result *= (pe - pe_f);                        /* p^e - p^{e-f} */
        }
    }
    return result;
}
