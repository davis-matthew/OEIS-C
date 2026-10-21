/*
 * A276283  Expansion of (1 + x + 3*x^2 + x^3)/((1 - x)^2*(1 + x^2)).
 * https://oeis.org/A276283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n - sin(pi*n/2) + 1
 * Range:     u8 n <= 85, u16 n <= 4444, u32 n <= 4444, u64 n <= 4444,
 *            u128 n <= 4444, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* sin(pi*n/2) is 0,1,0,-1 recurring for n mod 4 = 0,1,2,3 */
    idx_t mod4 = n & 3;        /* n % 4 without division */
    seq_t result = 3 * nn + 1; /* base value 3*n + 1 */

    if (mod4 == 1)
        result -= 1;          /* sin = 1  -> subtract 1 */
    else if (mod4 == 3)
        result += 1;          /* sin = -1 -> add 1 */

    /* for mod4 == 0 or 2 the base value is correct */
    return result;
}
