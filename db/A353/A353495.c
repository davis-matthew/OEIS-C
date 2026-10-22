/*
 * A353495  a(n) = 1 if the arithmetic derivative of n is of the form 4k+2,
 *          otherwise 0.
 * https://oeis.org/A353495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 iff arithmetic derivative A003415(n) ≡ 2 (mod 4),
 *            otherwise 0
 * Range:     u8 n <= 9377, u16 n <= 9377, u32 n <= 9377, u64 n <= 9377,
 *            u128 n <= 9377, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;   /* a(0)=0, a(1)=0 */

    /* factor n into primes and exponents */
    uint64_t pr[16];
    uint64_t ex[16];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t d = 0;
    seq_t N = (seq_t)n;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = (N / pr[i]) * ex[i];
        d += term;
    }

    return (d % 4 == 2) ? (seq_t)1 : (seq_t)0;
}
