/*
 * A303972  Total volume of all cubes with side length n which can be split
 *          such that n = p + q, p divides q and p < q.
 * https://oeis.org/A303972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 * Sum_{i=1..floor((n-1)/2)} floor((n-i)/i) -
 *            floor((n-i-1)/i)
 * Range:     u8 n <= 5, u16 n <= 23, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = (n - 1) / 2;          /* upper bound of the sum */
    idx_t sum = 0;

    for (idx_t i = 1; i <= m; ++i) {
        idx_t t1 = (n - i) / i;
        idx_t t2 = (n - i - 1) / i;
        sum += t1 - t2;            /* count of i such that i | (n-i) */
    }

    seq_t nn = (seq_t)n;
    seq_t result = nn * nn * nn * (seq_t)sum; /* n^3 * sum */
    return result;
}
