/*
 * A298364  Permutation of the natural numbers partitioned into quadruples
 *          [4k-2, 4k-1, 4k-3, 4k] for k > 0.
 * https://oeis.org/A298364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = base[(n-1)%4] + 4*((n-1)/4), base = [2,3,1,4]
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t base[4] = {(seq_t)2, (seq_t)3, (seq_t)1, (seq_t)4};
    /* n >= 1 */
    idx_t mod = (n - 1) % 4;
    idx_t q   = (n - 1) / 4;
    return base[mod] + (seq_t)4 * (seq_t)q;
}
