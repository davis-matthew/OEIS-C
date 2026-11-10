/*
 * A342798  Positions in A342753 of the reversal of the n-th word in A342753.
 * https://oeis.org/A342798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = position of reverse(w(n)) in A342753's word list, where
 *            w(1)=0, and w(n) = 0 w(n-1-floor((n-1)/3)) if 3 does not divide
 *            n, or w(n) = 1 w((n-3)/3) if 3 divides n. Words are read off and
 *            re-encoded by inverting the recursion
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A342798_child(idx_t n)
{
    idx_t a = n - 1;
    idx_t t = a / 3;
    return (n % 3 == 0) ? t : a - t;
}

seq_t a(idx_t n)
{
    idx_t idx = 0;
    while (n != 0) {
        int c = (n % 3 == 0);
        n = A342798_child(n);
        if (c)
            idx = 3 * (idx + 1);
        else
            idx = idx + 1 + idx / 2;
    }
    return (seq_t)idx;
}
