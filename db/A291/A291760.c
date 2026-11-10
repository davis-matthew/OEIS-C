/*
 * A291760  Binary encoding of 2-digits in ternary representation of
 *          A254103(n).
 * https://oeis.org/A291760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A289814(A254103(n)); A254103(0)=0,
 *            A254103(2n)=3*A254103(n)-1,
 *            A254103(2n+1)=floor(3*(1+A254103(n))/2); A289814(m) = binary
 *            number whose i-th bit is 1 iff the i-th ternary digit of m is 2
 * Range:     u8 n <= 63, u16 n <= 16383, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 129 terms through n = 9430 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A254103_a(idx_t n)
{
    seq_t v = 0;
    if (n == 0) return v;
    int h = 63;
    while (!((n >> h) & 1)) h--;
    for (int i = h; i >= 0; i--) {
        if ((n >> i) & 1) {
            v = ((seq_t)3 * (v + (seq_t)1)) / (seq_t)2;
        } else {
            v = (seq_t)3 * v - (seq_t)1;
        }
    }
    return v;
}

static seq_t A289814_a(seq_t m)
{
    seq_t res = 0;
    unsigned pos = 0;
    while (m > 0) {
        if (m % 3 == 2)
            res |= ((seq_t)1 << pos);
        m /= 3;
        pos++;
    }
    return res;
}

seq_t a(idx_t n)
{
    return A289814_a(A254103_a(n));
}
