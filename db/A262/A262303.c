/*
 * A262303  Length of sequence of lower halves of n: repeatedly apply
 *          x->floor(x/2) starting at n; a(n) = number of steps until reach
 *          one of 2,3,4.
 * https://oeis.org/A262303
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n∈{2,3,4}; otherwise a(n) = 1 + a(floor(n/2))
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;

    /* For the offset (n ≥ 2), the stopping set is {2,3,4}. */
    if (n <= 4)
        return 0;

    /* Repeatedly halve until n becomes 2, 3, or 4.  Count the steps. */
    while (n > 4) {
        n >>= 1;      /* equivalent to n = floor(n / 2) for unsigned n */
        ++res;
    }
    return res;
}
