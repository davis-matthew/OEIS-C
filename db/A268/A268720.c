/*
 * A268720  Row sums of A268719: a(n) = Sum_{k=0..n}
 *          A003188(A006068(n)+A006068(k)).
 * https://oeis.org/A268720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} A003188(A006068(n)+A006068(k)), with
 *            A003188(m) = m XOR floor(m/2), A006068(m) = m XOR floor(m/2) XOR
 *            floor(m/4) XOR
 * Range:     u8 n <= 12, u16 n <= 213, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 74 terms through n = 8155 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A003188_a(seq_t m)
{
    return m ^ (m >> 1);
}

static seq_t A006068_a(seq_t m)
{
    seq_t result = m;
    seq_t shifted = m;
    while (shifted > 0) {
        shifted >>= 1;
        result ^= shifted;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t gn = A006068_a((seq_t)n);
    seq_t total = 0;
    idx_t k;
    for (k = 0; k <= n; k++) {
        seq_t gk = A006068_a((seq_t)k);
        total += A003188_a(gn + gk);
    }
    return total;
}
