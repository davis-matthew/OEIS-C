/*
 * A255499  a(n) = (n^4 + 2*n^3 - n^2)/2.
 * https://oeis.org/A255499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4 + 2*n^3 - n^2)/2
 * Range:     u8 n <= 4, u16 n <= 18, u32 n <= 303, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a wider integer type for intermediate calculations
       so that the result is exact for all n that fit in seq_t. */
    unsigned __int128 nn  = (unsigned __int128)n;
    unsigned __int128 n2  = nn * nn;          /* n^2   */
    unsigned __int128 n3  = n2 * nn;          /* n^3   */
    unsigned __int128 n4  = n3 * nn;          /* n^4   */
    unsigned __int128 num = n4 + 2 * n3 - n2; /* numerator  */
    unsigned __int128 res = num / 2;          /* final value  */

    return (seq_t)res;
}
