/*
 * A372560  Array read by upward antidiagonals: A(n, k) = A371094(A(n-1, k))
 *          for n > 1, k >= 1; A(1, k) = A372443(k-1).
 * https://oeis.org/A372560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(n,k) = A371094(A(n-1,k)), A(1,k) = A372443(k-1)
 *            (A372443(0)=27, A372443(j)=odd part of 3*A372443(j-1)+1); read
 *            by upward antidiagonals: a(N) = A(d-i+1, i), N = d(d-1)/2 + i
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 15, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 28 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A371094_a(seq_t x)
{
    seq_t m = 3 * x + 1;
    seq_t t = m;
    seq_t p2 = 1;
    while ((t & 1) == 0) {
        t >>= 1;
        p2 <<= 1;
    }
    return m * p2 + (p2 * p2 - 1) / 3;
}

static seq_t A372443_a(idx_t j)
{
    seq_t x = 27;
    for (idx_t s = 0; s < j; s++) {
        x = 3 * x + 1;
        while ((x & 1) == 0) x >>= 1;
    }
    return x;
}

seq_t a(idx_t n)
{
    idx_t d = 1;
    idx_t rem = n;
    while (rem > d) {
        rem -= d;
        d++;
    }
    idx_t k = rem;
    idx_t row = d - rem + 1;
    seq_t x = A372443_a(k - 1);
    for (idx_t r = 1; r < row; r++)
        x = A371094_a(x);
    return x;
}
