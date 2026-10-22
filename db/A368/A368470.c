/*
 * A368470  a(n) is the number of exponentially odd divisors of the largest
 *          unitary divisor of n that is an exponentially odd number
 *          (A268335).
 * https://oeis.org/A368470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} (e+3)/2 if e is odd, 1 otherwise
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* i.e. a(n) is multiplicative with a(p^e) = (e+3)/2 when e odd, and 1 when e even */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* if n == 0 (should not happen in OEIS indexing) return 1 */
    if (n == 0) return 1;

    /* factor n into primes and exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        if (e & 1) {                /* exponent is odd */
            result *= ((seq_t)e + 3) / 2;  /* (e+3)/2 is integer */
        }
    }
    return result;
}
