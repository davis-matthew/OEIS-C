/*
 * A294302  Sum of the seventh powers of the parts in the partitions of n into
 *          two distinct parts.
 * https://oeis.org/A294302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1}^{⌊(n-1)/2⌋} (i^7 + (n-i)^7)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 21, u64 n <= 332, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2u)
        return 0;

    seq_t result = 0;
    idx_t g = (n - 1u) / 2u;          /* floor((n-1)/2) */
    seq_t sn = (seq_t)n;              /* cast index to seq_t once */

    for (idx_t i = 1; i <= g; ++i) {
        seq_t t = (seq_t)i;          /* i as seq_t */

        /* compute i^7 */
        seq_t t2 = t * t;            /* t^2 */
        seq_t t4 = t2 * t2;          /* t^4 */
        seq_t t7 = t4 * t2 * t;      /* t^7 = t^4 * t^2 * t */

        /* compute (n-i)^7 */
        seq_t p = sn - t;            /* n - i */
        seq_t p2 = p * p;            /* (n-i)^2 */
        seq_t p4 = p2 * p2;          /* (n-i)^4 */
        seq_t p7 = p4 * p2 * p;      /* (n-i)^7 = (n-i)^4 * (n-i)^2 * (n-i) */

        result += t7 + p7;
    }
    return result;
}
