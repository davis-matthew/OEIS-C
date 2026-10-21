/*
 * A296636  Sequences n*(n+1)*(6*n+1)/2 and n*(n+1)*(7*n+1)/2 interleaved.
 * https://oeis.org/A296636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0; For k ≥ 0: a(2k) = k*(k+1)*(7*k+1)/2 a(2k+1) =
 *            (k+1)*(k+2)*(6*(k+1)+1)/2
 * Range:     u8 n <= 7, u16 n <= 53, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    if (n & 1) {                     /* odd indices */
        seq_t k = ((seq_t)(n + 1)) / 2;   /* k = (n+1)/2 */
        return k * (k + 1) * (6 * k + 1) / 2;
    } else {                          /* even indices */
        seq_t k = (seq_t)(n / 2);          /* k = n/2 */
        return k * (k + 1) * (7 * k + 1) / 2;
    }
}
