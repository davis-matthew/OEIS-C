/*
 * A400361  a(n) = A018804(n) - A384628(n).
 * https://oeis.org/A400361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   a(n) = sum_{k=1..n} gcd(n,k) - sum_{k=1..n} gcd(n, floor(n/k))
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function works for any idx_t>=1 and any seq_t type, using only integer arithmetic. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* compute S1 = sum_{k=1..n} gcd(n,k) */
    seq_t S1 = 0;
    for (idx_t k = 1; k <= n; ++k) {
        S1 += gcd((seq_t)n, (seq_t)k);
    }

    /* compute H = sum_{k=1..n} gcd(n, floor(n/k)) via quotient grouping */
    seq_t H = 0;
    for (idx_t l = 1; l <= n; ) {
        idx_t q = n / l;            /* floor(n/l) */
        idx_t r = n / q;            /* last k with same floor */
        seq_t cnt = (seq_t)(r - l + 1);
        seq_t g   = gcd((seq_t)n, (seq_t)q);
        H += g * cnt;
        l = r + 1;
    }

    return S1 - H;
}
