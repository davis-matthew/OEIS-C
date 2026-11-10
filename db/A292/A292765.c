/*
 * A292765  A292764(n)/2.
 * https://oeis.org/A292765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = A292764(n)/2, where A292764 satisfies A292764(n) =
 *            A292764(n-1) + A292764(n-2) + A292764(n-3) - 2*A292764(n-5) for
 *            n > 10, with A292764(1..10) = 2, 8, 18, 36, 66, 120, 210, 360,
 *            618, 1052
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 16 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* A292764: minimal number of moves for the cyclic 4-peg Hanoi tower,
   final peg two steps away (copied from the validated implementation). */
static seq_t A292764_a(idx_t n)
{
    static const unsigned A292764_init[10] = {
        2u, 8u, 18u, 36u, 66u, 120u, 210u, 360u, 618u, 1052u
    };
    if (n == 0) return (seq_t)0;
    if (n <= 10) return (seq_t)A292764_init[n - 1];

    seq_t w[5]; /* w[0] = A292764(i-5) ... w[4] = A292764(i-1) */
    for (int k = 0; k < 5; ++k) w[k] = (seq_t)A292764_init[5 + k];
    for (idx_t i = 11; i <= n; ++i) {
        seq_t v = w[4] + w[3] + w[2];
        v -= (seq_t)2 * w[0];
        w[0] = w[1]; w[1] = w[2]; w[2] = w[3]; w[3] = w[4]; w[4] = v;
    }
    return w[4];
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    return A292764_a(n) / 2;
}
