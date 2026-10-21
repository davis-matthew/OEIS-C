/*
 * A274339  The period 3 sequence of the iterated sum of deficient divisors
 *          function (A187793) starting at 15.
 * https://oeis.org/A274339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+3) = a(n) (period 3) Initial values: a(1) = 15, a(2) = 24,
 *            a(3) = 18
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /*  n is 1‑based */
    idx_t r = n % 3;
    if (r == 1) return (seq_t)15;
    if (r == 2) return (seq_t)24;
    return (seq_t)18;   /* r == 0 */
}
