/*
 * A324272  a(n) = 2*13^(2*n).
 * https://oeis.org/A324272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 2*13^(2*n)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 2;                       /* initial factor 2 */
    seq_t exp = (seq_t)n * 2;               /* exponent 2*n */
    seq_t base = 13;                        /* base 13 */
    seq_t power = 1;                        /* accumulator for base^exp */

    while (exp > 0) {
        if (exp & 1) power *= base;        /* multiply by current base if bit = 1 */
        exp >>= 1;                         /* move to next lower bit */
        if (exp) base *= base;             /* square base only if more bits remain */
    }

    result *= power;                       /* final result */
    return result;
}
