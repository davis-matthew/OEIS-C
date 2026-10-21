/*
 * A288205  Positions of 1 in A288203; complement of A288204.
 * https://oeis.org/A288205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n - floor(log2(n)) - 1 (equivalently 4*n - (num of
 *            powers of 2 ≤ n))
 * Range:     u8 n <= 65, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* idx_t is uint64_t, so we can safely use builtin clzll for log2(n) */
    unsigned int l = 63 - __builtin_clzll(n);      /* floor(log2(n)) */
    seq_t res = (seq_t)n * 4 - (seq_t)(l + 1);     /* subtract number of powers of 2 ≤ n */
    return res;
}
