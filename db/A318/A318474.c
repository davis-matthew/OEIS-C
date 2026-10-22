/*
 * A318474  Multiplicative with a(p^e) = 2^A000045(e+1).
 * https://oeis.org/A318474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Π_{p^e‖n} 2^Fib(e+1) (Fib = Fibonacci numbers)
 * Range:     u8 n <= 31, u16 n <= 127, u32 n <= 255, u64 n <= 1023,
 *            u128 n <= 2047, big any n
 * Helpers:   factor_u64, fib_uint, pow_uint, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_mul.h"
#include "../utils/pow_uint.h"
#include "../utils/fib_uint.h"

/* Multiplicative function A318474 */
seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)1;

    /* Factor n into primes and exponents */
    uint64_t primes[64], exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        idx_t e = (idx_t)exps[i];
        uint64_t fib_val = fib_uint(e + 1);        /* Fib(e+1) */
        seq_t term      = pow_uint(2, (idx_t)fib_val); /* 2^Fib(e+1) */
        result = seq_mul(result, term);
    }
    return result;
}
