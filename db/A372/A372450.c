/*
 * A372450  a(n) = k, if A086893(k) is the first term of A086893 reached on
 *          the trajectory of reduced Collatz-function R, when starting from
 *          2n-1, or -1 if no term of A086893 is ever encountered.
 * https://oeis.org/A372450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k where A086893(k) is the first term of A086893 on the
 *            trajectory of x -> A000265(3x+1) started at 2n-1; A086893(k+2) =
 *            4*A086893(k)+1 with A086893(1)=1, A086893(2)=3
 * Range:     u8 n <= 13, u16 n <= 959, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 124 terms through n = 9806 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A372450_find(seq_t x)
{
    /* index k of x in A086893, or 0 if x is not a term */
    seq_t t = 1;
    seq_t k = 1;
    for (;;) {
        if (t == x) return k;
        if (t > x || t > (x - 1) / 4) break;
        t = 4 * t + 1;
        k += 2;
    }
    t = 3;
    k = 2;
    for (;;) {
        if (t == x) return k;
        if (t > x || t > (x - 1) / 4) break;
        t = 4 * t + 1;
        k += 2;
    }
    return 0;
}

seq_t a(idx_t n)
{
    seq_t x = 2 * (seq_t)n - 1;
    for (long it = 0; it < 1000000; it++) {
        seq_t k = A372450_find(x);
        if (k != 0) return k;
        x = 3 * x + 1;
        while ((x & 1) == 0) x >>= 1;
    }
    return (seq_t)0 - 1;
}
