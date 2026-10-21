/*
 * A291568  The arithmetic function uhat(n,5,5).
 * https://oeis.org/A291568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-5) for n > 5 Initial values: a(1)=1, a(2)=1, a(3)=1,
 *            a(4)=1, a(5)=-3
 * Range:     i8 n <= 70, i16 n <= 70, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Since seq_t may be signed, cast constants appropriately. */
    const seq_t ONE = (seq_t)1;
    const seq_t NEG3 = (seq_t)-3;

    /* n is 1-based, period 5 */
    idx_t r = (n - 1) % 5ul;  /* 0..4 */
    if (r == 4)
        return NEG3;
    else
        return ONE;
}
