/*
 * A284552  a(n) = A065621(n) modulo n.
 * https://oeis.org/A284552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A065621(n) mod n, where A065621(n) = n XOR (2*(n -
 *            lowbit(n)))
 * Range:     u8 n <= 257, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 110 terms through n = 7613 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A065621_a(idx_t n)
{
    seq_t n_t = (seq_t)n;
    idx_t low = n & -n;
    seq_t low_t = (seq_t)low;
    return n_t ^ ((n_t - low_t) << 1);
}

seq_t a(idx_t n)
{
    seq_t v = A065621_a(n);
    return v % (seq_t)n;
}
