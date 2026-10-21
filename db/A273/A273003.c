/*
 * A273003  Arrange the base 10 digits of the n-th Fibonacci number in
 *          descending order.
 * https://oeis.org/A273003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A004186(A000045(n))
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/digit_counts.h"

/* Compute the n-th Fibonacci number (F(0)=0, F(1)=1). */
static seq_t fib_seq(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    seq_t a = (seq_t)0;
    seq_t b = (seq_t)1;
    for (idx_t i = 1; i < n; ++i) {
        seq_t c = a + b;
        a = b;
        b = c;
    }
    return b;
}

/* a(n) = arrange the base‑10 digits of the n‑th Fibonacci number
 *         in descending order. */
seq_t a(idx_t n)
{
    seq_t f = fib_seq(n);

    unsigned char cnt[10] = {0};
    digit_counts(f, cnt);        /* counts decimal digits of f */

    seq_t result = (seq_t)0;
    for (int d = 9; d >= 0; --d) {
        for (unsigned char c = 0; c < cnt[d]; ++c) {
            result = result * (seq_t)10 + (seq_t)d;
        }
    }
    return result;
}
