/*
 * A277823  a(n) = A048724(A065621(n)).
 * https://oeis.org/A277823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A048724(A065621(n)), where A065621(n) = n XOR ((n -
 *            lowbit(n)) << 1) and A048724(m) = m XOR (m << 1)
 * Range:     u8 n <= 64, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 91 terms through n = 7843 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A065621_a(idx_t n)
{
    seq_t n_t = (seq_t)n;
    idx_t low = n & (idx_t)(0 - n);
    seq_t low_t = (seq_t)low;
    return n_t ^ ((n_t - low_t) << 1);
}

static seq_t A048724_a(seq_t m)
{
    return m ^ (m << 1);
}

seq_t a(idx_t n)
{
    return A048724_a(A065621_a(n));
}
