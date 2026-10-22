/*
 * A394510  a(2*k) = 2*k*(5*k+1)*(10*k+1), a(2*k+1) =
 *          2*(k+1)*(5*k+4)*(10*k+9), with k >= 0.
 * https://oeis.org/A394510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2*k) = 2*k*(5*k+1)*(10*k+1), a(2*k+1) =
 *            2*(k+1)*(5*k+4)*(10*k+9), k>=0
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t half = n >> 1;          /* floor(n/2) */
    if ((n & 1) == 0) {           /* even n */
        seq_t k   = (seq_t)half;
        seq_t res = (seq_t)2 * k;
        res *= (seq_t)(5 * k + 1);
        res *= (seq_t)(10 * k + 1);
        return res;
    } else {                      /* odd n */
        seq_t k   = (seq_t)half;
        seq_t k1  = k + 1;
        seq_t res = (seq_t)2 * k1;
        res *= (seq_t)(5 * k + 4);
        res *= (seq_t)(10 * k + 9);
        return res;
    }
}
