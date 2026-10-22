/*
 * A394678  a(n) = (n! - 1) mod n^2.
 * https://oeis.org/A394678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n! - 1) mod n^2
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* ensure we don't overflow in the intermediate multiplication */
    if (n == 0) return 0;                /* guard – offset is 1 */

    seq_t sq = (seq_t)n * (seq_t)n;      /* n^2  (fits for all practical n) */
    seq_t fact = (seq_t)1;               /* current factorial modulo n^2 */

    for (idx_t i = 2; i <= n; ++i)
        fact = (fact * (seq_t)i) % sq;   /* multiply and reduce modulo n^2 */

    /* (fact - 1) mod n^2 – avoid negative values */
    seq_t res = (fact + sq - (seq_t)1) % sq;
    return res;
}
