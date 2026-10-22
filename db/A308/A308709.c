/*
 * A308709  Start with 3, divide by 3, multiply by 2, multiply by 3, multiply
 *          by 2, repeat.
 * https://oeis.org/A308709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   start at 3, then cycle /3, *2, *3, *2 for successive terms
 *            (a(1)=3)
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)3;          /* a(1) = 3 */
    seq_t cur = (seq_t)3;                 /* current term */

    for (idx_t i = 2; i <= n; ++i) {
        /* (i-2) mod 4 determines the operation:
           0 → divide by 3
           1 → multiply by 2
           2 → multiply by 3
           3 → multiply by 2          */
        switch ((i - 2) & 3) {
            case 0: cur /= (seq_t)3; break;
            case 1: cur *= (seq_t)2; break;
            case 2: cur *= (seq_t)3; break;
            default: cur *= (seq_t)2; break;   /* case 3 */
        }
    }
    return cur;
}
