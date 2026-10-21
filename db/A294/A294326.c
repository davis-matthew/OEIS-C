/*
 * A294326  a(n) = Product_{k=0..n} (5*k + 4)!.
 * https://oeis.org/A294326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=0..n} (5*k + 4)!
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) = 4! = 24 */
    seq_t f = (seq_t)24;     /* current factorial (5*k+4)!  */
    seq_t result = f;        /* running product of all factorials */

    for (idx_t k = 1; k <= n; ++k) {
        /* update f from (5*(k-1)+4)! to (5*k+4)! by multiplying
           the five new terms 5*k, 5*k+1, …, 5*k+4               */
        seq_t base = (seq_t)5 * k;
        for (uint_fast8_t j = 0; j < 5; ++j) {
            f *= (base + j);
        }
        /* multiply the new factorial into the overall product   */
        result *= f;
    }
    return result;
}
