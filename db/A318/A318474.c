/*
 * A318474  Multiplicative with a(p^e) = 2^A000045(e+1).
 * https://oeis.org/A318474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Π_{p^e‖n} 2^Fib(e+1) (Fib = Fibonacci numbers)
 * Range:     u8 n <= 31, u16 n <= 127, u32 n <= 255, u64 n <= 1023,
 *            u128 n <= 2047, big any n
 * Validated: standard, pass, 121 terms through n = 9964 (2026-11-01)
 * Helpers:   factor_u64, fibonacci_idx, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/factor_u64.h"
#include "../utils/fibonacci_idx.h"
#include "../utils/ipow_u64_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

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
        uint64_t fib_val = fibonacci_idx(e + 1);        /* Fib(e+1) */
        seq_t term      = ipow_u64_idx(2, (idx_t)fib_val); /* 2^Fib(e+1) */
        result = seq_mul(result, term);
    }
    return result;
}
