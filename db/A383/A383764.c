/*
 * A383764  The largest unitary divisor of n that is an exponentially
 *          squarefree number.
 * https://oeis.org/A383764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Π_{p^e‖n, e squarefree} p^e (Multiplicative with a(p^e) =
 *            p^e if e is squarefree, and 1 otherwise.)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, is_sqfree_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/is_sqfree_uint64.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* undefined for n < 1, return 0 safely */
    uint64_t un = (uint64_t)n;
    if (un == 1) return (seq_t)1;            /* a(1) = 1 */

    /* Factorization: obtain primes and corresponding exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(un, primes, exps, &cnt);

    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        if (is_sqfree_uint64(e)) {
            seq_t term = 1;
            for (uint64_t k = 0; k < e; ++k) {
                term *= (seq_t)p;           /* p^e, e ≤ 63 for 64-bit n */
            }
            res *= term;
        }
    }
    return res;
}
