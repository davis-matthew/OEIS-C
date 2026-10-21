/*
 * A281066  Concatenation R(n)R(n-1)R(n-2)...R(2)R(1) read mod n, where R(x)
 *          is the digit-reversal of x (with leading zeros not omitted).
 * https://oeis.org/A281066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look, base
 * Formula:   a(n) = A138793(n) mod n, where A138793(n) is the integer
 *            obtained by concatenating the digit‑reversals of n , n‑1, …, 1
 *            (with leading zeros from trailing zeros preserved)
 * Range:     u8 n <= 269, u16 n <= 9794, u32 n <= 9794, u64 n <= 9794,
 *            u128 n <= 9794, big any n
 * Helpers:   mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mod_mul.h"

seq_t a(idx_t n)
{
    /* work in 64‑bit arithmetic to avoid overflow when multiplying by 10
       and afterward reduce modulo n using the provided mod_mul routine. */
    uint64_t mod = (uint64_t)n;          /* n ≥ 1 by definition of the sequence */
    uint64_t res = 0;

    for (idx_t i = n; i >= 1; --i) {
        idx_t x = i;
        /* process digits of i from least significant to most significant
           (which yields the reversed representation, preserving leading
           zeros that arise from trailing zeros of the original number). */
        do {
            uint64_t d = x % 10;
            res = (mod_mul(res, 10, mod) + d) % mod;
            x /= 10;
        } while (x > 0);
    }

    return (seq_t)res;
}
