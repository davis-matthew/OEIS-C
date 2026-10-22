/*
 * A342634  a(0) = 0, a(1) = 1; a(2*n) = a(n), a(2*n+1) = 4*a(n) + a(n+1).
 * https://oeis.org/A342634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1; a(2*n) = a(n); a(2*n+1) = 4*a(n) + a(n+1)
 * Range:     u8 n <= 30, u16 n <= 510, u32 n <= 9871, u64 n <= 9871,
 *            u128 n <= 9871, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    if ((n & 1ULL) == 0) {          /* even index */
        return a(n >> 1);
    } else {                        /* odd index */
        idx_t half   = n >> 1;
        seq_t  val1  = a(half);
        seq_t  val2  = a(half + 1);
        seq_t  four  = (seq_t)4;
        return four * val1 + val2;
    }
}
