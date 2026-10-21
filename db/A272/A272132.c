/*
 * A272132  a(n) = 6144*n^4 - 29184*n^3 + 52416*n^2 - 41840*n + 12465.
 * https://oeis.org/A272132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6144*n^4 - 29184*n^3 + 52416*n^2 - 41840*n + 12465
 * Range:     u8 none, u16 n <= 2, u32 n <= 30, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast index to seq_t early to avoid 64‑bit overflow in intermediate
       computations for sequences that fit in 64 bits.  All arithmetic is
       performed on seq_t, which may be arbitrary precision. */
    seq_t s  = (seq_t)n;
    seq_t n2 = s * s;          /* n^2 */
    seq_t n3 = n2 * s;         /* n^3 */
    seq_t n4 = n2 * n2;        /* n^4 */

    /* Compute the positive and negative parts separately to avoid
       intermediate negative values when seq_t is unsigned. */
    seq_t pos = (seq_t)12465
                + (seq_t)52416 * n2
                + (seq_t)6144  * n4;
    seq_t neg = (seq_t)29184 * n3
                + (seq_t)41840 * s;

    return pos - neg;
}
