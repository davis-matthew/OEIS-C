/*
 * A281237  Solutions x to the negative Pell equation y^2 = 72*x^2 - 73728
 *          with x,y >= 0.
 * https://oeis.org/A281237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - a(n-2), a(1)=32, a(2)=96
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly */
    if (n == 1) return (seq_t)32;
    if (n == 2) return (seq_t)96;

    /* Iterate from the second term up to n */
    seq_t prev = (seq_t)32;  /* a(1) */
    seq_t curr = (seq_t)96;  /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = (seq_t)(6 * curr - prev);
        prev = curr;
        curr = next;
    }

    return curr;
}
