/*
 * A337821  For n >= 0, a(4n+1) = 0, a(4n+3) = a(2n+1) + 1, a(2n+2) = a(n+1).
 * https://oeis.org/A337821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: for n >= 0
 *   a(4n+1) = 0
 *   a(4n+3) = a(2n+1) + 1
 *   a(2n+2) = a(n+1)
 * Initial values: a(1) = 0, a(2) = 0
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    while (1) {
        switch (n & 3) {          /* n % 4 */
        case 1:                   /* a(4k+1) = 0 */
            return res;
        case 3:                   /* a(4k+3) = a((n-1)/2) + 1 */
            res += 1;
            n = (n - 1) >> 1;     /* (n-1)/2 */
            break;
        default:                  /* even indices: a(2k) = a(k) */
            n >>= 1;              /* n/2 */
            break;
        }
    }
}
