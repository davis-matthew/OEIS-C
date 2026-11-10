/*
 * A372361  Array read by upward antidiagonals: A(n, k) = A372358(A372283(n,
 *          k)), n,k >= 1.
 * https://oeis.org/A372361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A372358(A372283(n)), where A372358(m) = m XOR
 *            A086893(bitlength(m)), and A372283 is read by upward
 *            antidiagonals from A(1,k)=2k-1, A(n,k)=odd part of 3*A(n-1,k)+1
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static seq_t A372361_oddpart(seq_t x)
{
    if (x == 0) return 0;
    while ((x & 1) == 0) x >>= 1;
    return x;
}

static seq_t A372361_A372283(idx_t n)
{
    idx_t d = 1;
    while (d * (d + 1) / 2 < n) ++d;
    idx_t p = n - d * (d - 1) / 2;
    idx_t row = d - p + 1;
    seq_t v = (seq_t)2 * (seq_t)p - (seq_t)1;
    for (idx_t r = 2; r <= row; ++r)
        v = A372361_oddpart((seq_t)3 * v + (seq_t)1);
    return v;
}

static seq_t A372361_A372358(seq_t m)
{
    unsigned L = 0;
    seq_t t = m;
    while (t > 0) { t >>= 1; ++L; }
    seq_t pat = 0;
    for (unsigned j = 0; j < L; ++j) {
        unsigned bit;
        if (L & 1) bit = ((j & 1) == 0);
        else bit = (j == 0 || (j & 1) == 1);
        pat = (pat << 1) | (seq_t)bit;
    }
    return m ^ pat;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    return A372361_A372358(A372361_A372283(n));
}
