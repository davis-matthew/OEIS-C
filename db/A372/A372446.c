/*
 * A372446  a(n) = A372358(A372443(n)).
 * https://oeis.org/A372446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A372358(A372443(n)) = x XOR A086893(bitlength(x)), where
 *            x = A372443(n): x(0) = 27, x(k) = odd part of 3*x(k-1)+1;
 *            A086893(1)=1, A086893(2)=3, A086893(L)=4*A086893(L-2)+1
 * Range:     u8 n <= 5, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 72 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A086893_pat(unsigned L)
{
    seq_t val;
    unsigned i;
    if (L == 0) return (seq_t)0;
    if (L & 1) { val = (seq_t)1; i = 3; }
    else       { val = (seq_t)3; i = 4; }
    for (; i <= L; i += 2)
        val = (seq_t)4 * val + (seq_t)1;
    return val;
}

seq_t a(idx_t n)
{
    seq_t x = (seq_t)27;
    for (idx_t k = 0; k < n; ++k) {
        x = (seq_t)3 * x + (seq_t)1;
        while ((x & 1) == 0)
            x >>= 1;
    }
    unsigned L = 0;
    seq_t t = x;
    while (t > 0) {
        ++L;
        t >>= 1;
    }
    return x ^ A086893_pat(L);
}
