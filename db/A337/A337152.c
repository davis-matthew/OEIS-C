/*
 * A337152  a(n) = 2^n * (n!)^2 * Sum_{k=0..n} 1 / ((-2)^k * (k!)^2).
 * https://oeis.org/A337152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n * (n!)^2 * Sum_{k=0..n} 1 / ((-2)^k * (k!)^2)
 *            Recurrence: a(0) = 1; a(n) = 2*n^2*a(n-1)+(-1)^n
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 1;
    for (idx_t i = 1; i <= n; ++i) {
        /* val = 2 * i^2 * val  */
        val = ((seq_t)2 * (seq_t)i * (seq_t)i) * val;
        /* add (+1) if i even, subtract 1 if i odd */
        val = (i & 1) ? (val - (seq_t)1) : (val + (seq_t)1);
    }
    return val;
}
