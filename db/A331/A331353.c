/*
 * A331353  Number of achiral colorings of the edges (or triangular faces) of
 *          a regular 4-dimensional simplex with n available colors.
 * https://oeis.org/A331353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n^3 + n^7) / 6 This implementation uses 128‑bit
 *            arithmetic to avoid overflow while still working with any of the
 *            expected seq_t types (uint8/16/32/64, unsigned __int128 or an
 *            arbitrary‑precision class)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 30, u64 n <= 730, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 0 we return 0; the sequence is defined for n >= 1. */
    if (n == 0) return 0;

    /* Use a 128‑bit signed type for intermediate calculations.
       __int128 is available on the compilers used for the test harnesses. */
    __int128 nn   = (__int128)n;     /* cast index to 128‑bit */
    __int128 n3   = nn * nn * nn;    /* n^3 */
    __int128 n4   = n3 * nn;         /* n^4  (= n^3 * n) */
    __int128 q    = n4 + 5;          /* n^4 + 5 */
    __int128 prod = n3 * q;          /* n^3 * (n^4 + 5) = n^7 + 5n^3 */
    __int128 res  = prod / 6;        /* division by 6 is exact */

    /* Cast back to the generic return type seq_t. */
    return (seq_t)res;
}
