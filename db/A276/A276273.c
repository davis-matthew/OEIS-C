/*
 * A276273  Replacing every "mixed pair" of integers (as defined in the
 *          comments) with the smaller integer of the pair rebuilds the
 *          sequence.
 * https://oeis.org/A276273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A000120(A064706(n-1)) + 1, where A064706(m) = m XOR
 *            floor(m/4)
 * Range:     u8 n <= 563, u16 n <= 10004, u32 n <= 10004, u64 n <= 10004,
 *            u128 n <= 10004, big any n
 * Validated: standard, pass, 147 terms through n = 8952 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A000120_a(idx_t n)
{
    seq_t c = 0;
    while (n) {
        c += (seq_t)(n & 1);
        n >>= 1;
    }
    return c;
}

static seq_t A064706_a(idx_t n)
{
    return (seq_t)(n ^ (n / 4));
}

seq_t a(idx_t n)
{
    idx_t m = n - 1;
    idx_t g = (idx_t)(A064706_a(m));
    return A000120_a(g) + 1;
}
