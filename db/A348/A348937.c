/*
 * A348937  a(n) = A003961(n) - A003415(n), where A003961 shifts the prime
 *          factorization of n one step towards larger primes, and A003415
 *          gives the arithmetic derivative of n.
 * https://oeis.org/A348937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003961(n) - A003415(n)
 * Range:     u8 n <= 47, u16 n <= 1535, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 92 terms through n = 9951 (2026-11-01)
 * Helpers:   factor_u64, ipow_i32, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_i32.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* Handle the offset (a(1) is the first term). */
    if (n == 0)                /* value for index 0 is irrelevant */
        return 0;

    seq_t N = (seq_t)n;        /* n as seq_t for the calculations */

    /* Factorize n. */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t shift = 1;           /* result of A003961(n)   */
    seq_t deriv = 0;           /* result of A003415(n)   */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p  = primes[i];
        uint64_t ep = exps[i];

        /* Next prime after p. */
        uint64_t np = next_prime(p);

        /* Multiply shift by np^ep. */
        seq_t powp = ipow_i32((seq_t)np, (int)ep);
        shift = shift * powp;

        /* Derivative contribution: ep * (N/p). */
        deriv = deriv + ((N / (seq_t)p) * (seq_t)ep);
    }

    return shift - deriv;
}
