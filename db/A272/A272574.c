/*
 * A272574  a(n) = f(9, f(8, n)), where f(k,m) = floor(m*k/(k-1)).
 * https://oeis.org/A272574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(floor(n*8/7) * 9/8)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: a(n) = (63*n - 12 - 12*(n mod 7) + 2*((-n-1) mod 7))/49 */

seq_t a(idx_t n)
{
    /* Compute t = floor(n*8/7)  */
    seq_t t = ((seq_t)n * (seq_t)8) / (seq_t)7;

    /* Compute a(n) = floor(t*9/8)  – avoid overflow by splitting */
    seq_t res = (t / (seq_t)8) * (seq_t)9 + ((t % (seq_t)8) * (seq_t)9) / (seq_t)8;

    return res;
}
