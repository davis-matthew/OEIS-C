/*
 * A286633  Base-3 {digit+1} product of A254103: a(n) = A006047(A254103(n)).
 * https://oeis.org/A286633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A006047(A254103(n)); A254103: a(0)=0, a(2n)=3*a(n)-1,
 *            a(2n+1)=floor(3*(1+a(n))/2); A006047(m) = 2^(#1s in base 3 of m)
 *            * 3^(#2s)
 * Range:     u8 n <= 63, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 112 terms through n = 8117 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A254103_a(idx_t n)
{
    seq_t v = (seq_t)0;
    int i;
    int started = 0;
    for (i = 63; i >= 0; --i) {
        int bit = (int)((n >> i) & 1);
        if (!started) {
            if (!bit) continue;
            started = 1;
        }
        if (bit) v = ((seq_t)3 * (v + (seq_t)1)) / (seq_t)2;
        else     v = (seq_t)3 * v - (seq_t)1;
    }
    return v;
}

static seq_t A006047_a(seq_t m)
{
    seq_t result = (seq_t)1;
    while (m > (seq_t)0) {
        seq_t d = m % (seq_t)3;
        if (d == (seq_t)1) result = result * (seq_t)2;
        else if (d == (seq_t)2) result = result * (seq_t)3;
        m = m / (seq_t)3;
    }
    return result;
}

seq_t a(idx_t n)
{
    return A006047_a(A254103_a(n));
}
