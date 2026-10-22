/*
 * A377729  a(n) is the smallest number which can be represented as the sum of
 *          n distinct nonzero n-gonal numbers in exactly 2 ways.
 * https://oeis.org/A377729
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 - 2*n^3 + 38*n^2 - 85*n + 72)/6 for n >= 5 a(3) =
 *            19, a(4) = 90 (This returns the value of OEIS sequence A377729.)
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return 0;          /* sequence is defined for n >= 3 */
    if (n == 3) return 19;
    if (n == 4) return 90;

    seq_t nn = (seq_t)n;

    /* compute the polynomial: (n^4 - 2*n^3 + 38*n^2 - 85*n + 72)/6 */
    seq_t val = nn * nn * nn * nn;          /* n^4 */
    val -= 2 * nn * nn * nn;                /* -2*n^3 */
    val += 38 * nn * nn;                    /* +38*n^2 */
    val -= 85 * nn;                         /* -85*n */
    val += 72;                              /* +72 */
    val /= 6;                               /* divide by 6 */

    return val;
}
