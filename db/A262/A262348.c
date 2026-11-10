/*
 * A262348  a(n) = A000215(n) mod A000045(n).
 * https://oeis.org/A262348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000215(n) mod A000045(n), where A000215(n) = 2^(2^n) + 1
 *            and A000045 is the Fibonacci numbers (F(0)=0, F(1)=1,
 *            F(k)=F(k-1)+F(k-2)); computed by n modular squarings of 2
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-07)
 * Helpers:   mulmod_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mulmod_seq.h"

static seq_t A000045_a(idx_t n)
{
    seq_t x = 0, y = 1;
    idx_t i;
    for (i = 0; i < n; i++) {
        seq_t t = x + y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n)
{
    seq_t m = A000045_a(n);
    seq_t x;
    idx_t i;
    if (m == 0)
        return 0;
    x = (seq_t)2 % m;
    for (i = 0; i < n; i++)
        x = mulmod_seq(x, x, m);
    return (x + 1) % m;
}
