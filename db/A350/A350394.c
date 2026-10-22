/*
 * A350394  Largest degree of x with the largest coefficient in
 *          Product_{k=1..n} (1 + x^k).
 * https://oeis.org/A350394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(n+1)/4) + ((n mod 4 == 1 or 2) ? 1 : 0) for n >=
 *            12. For n < 12 the values are hard‐coded from the OEIS sequence.
 *            This implements OEIS sequence A350394
 * Range:     u8 n <= 31, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* first 12 values, index 0..11 */
    static const seq_t small[12] = {
        0, 1, 3, 3, 7, 10, 12, 16, 18, 24, 28, 33
    };

    if (n < 12)
        return small[n];

    /* compute floor(n*(n+1)/4) using seq_t arithmetic */
    seq_t m = (seq_t)n * (seq_t)(n + 1) / 4;

    /* add 1 if n mod 4 is 1 or 2, else add 0 */
    idx_t r = n % 4;
    seq_t diff = ((r == 1) || (r == 2)) ? 1 : 0;

    return m + diff;
}
