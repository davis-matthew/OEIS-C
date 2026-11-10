/*
 * A292241  The 3-adic valuation of A254103(n).
 * https://oeis.org/A292241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3-adic valuation of A254103(n); A254103(0)=0,
 *            A254103(2n)=3*A254103(n)-1,
 *            A254103(2n+1)=floor(3*(1+A254103(n))/2); a(0)=0
 * Range:     u8 n <= 95, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 137 terms through n = 9703 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t v = 0;
    int top = -1;
    int i;
    for (i = 63; i >= 0; --i) {
        if ((n >> i) & 1) { top = i; break; }
    }
    for (i = top; i >= 0; --i) {
        if ((n >> i) & 1) {
            v = ((seq_t)3 * (v + (seq_t)1)) / (seq_t)2;
        } else {
            v = (seq_t)3 * v - (seq_t)1;
        }
    }
    if (v == 0) return (seq_t)0;
    seq_t count = 0;
    while (v % 3 == 0) {
        count++;
        v /= 3;
    }
    return count;
}
