/*
 * A288920  Number of 6-cycles in the n X n king graph.
 * https://oeis.org/A288920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 550 - 534*n + 126*n^2 (valid for n > 2) a(1) = 0, a(2) =
 *            0, a(3) = 82, a(4) = 430
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset and small special cases */
    if (n <= 2)
        return 0;
    if (n == 3)
        return (seq_t)82;
    if (n == 4)
        return (seq_t)430;

    /* For n >= 5 the quadratic formula never underflows,
       so plain arithmetic in seq_t is safe. */
    seq_t nn   = (seq_t)n * (seq_t)n;      /* n^2 */
    seq_t term = 126 * nn;                 /* 126*n^2 */
    term -= 534 * (seq_t)n;                /* -534*n */
    term += 550;                           /* +550 */
    return term;
}
