/*
 * A391584  Odd numbers k such that 3*k = A048720(m,k) for some m, where
 *          A048720 is carryless base-2 multiplication.
 * https://oeis.org/A391584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth odd integer k such that there exists an m with
 *            carry‑less base‑2 multiplication A048720(m,k) = 3*k. The
 *            function tests each odd k whether the binary representation of
 *            3*k (ordinary multiplication) is divisible by k as polynomials
 *            over GF(2)
 * Range:     u8 n <= 38, u16 n <= 1786, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_u128, divisible128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/bitlen_u128.h"
#include "../utils/divisible128.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n < 1 */
    seq_t k = 1;                          /* first odd integer */
    idx_t count = 0;

    while (1) {
        __uint128_t k128 = (__uint128_t)k;
        __uint128_t r128 = k128 * (__uint128_t)3;  /* ordinary 3*k */

        if (divisible128(k128, r128)) {
            ++count;
            if (count == n) return k;
        }
        k += 2;                            /* next odd candidate */
    }
}
