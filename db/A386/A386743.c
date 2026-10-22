/*
 * A386743  For n >= 0, a(n) is the least Fibonacci number F(i) such that F(i)
 *          = (2*n + 1)*(n + k) for some k >= 0.
 * https://oeis.org/A386743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least Fibonacci F(i) such that F(i) = (2*n + 1)*(n + k)
 *            for some k >= 0
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 36, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute d = 2*n + 1 using seq_t arithmetic */
    seq_t d = (seq_t)n * 2 + 1;

    /* Fibonacci iteration: F(0) = 0, F(1) = 1 */
    seq_t cur = 0;
    seq_t next = 1;

    for (idx_t i = 0; ; ++i) {
        /* test current Fibonacci number */
        if (cur % d == 0) {
            seq_t q = cur / d;
            if (q >= (seq_t)n) {
                return cur;           /* found minimal F(i) */
            }
        }

        /* advance to next Fibonacci number */
        seq_t tmp = cur + next;
        cur = next;
        next = tmp;
    }
}
