/*
 * A256008  Self-inverse permutation of positive integers: 4k+1 is swapped
 *          with 4k+3, and 4k+2 with 4k+4.
 * https://oeis.org/A256008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 2 if n mod 4 is 1 or 2; otherwise a(n) = n – 2
 * Range:     u8 n <= 253, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1-based index; only values for n >= 1 matter. */
    idx_t r = n & 3;          /* r = n % 4  (use bitwise for speed) */
    if (r == 1 || r == 2)    /* positions 4k+1 and 4k+2 */
        return (seq_t)n + 2;
    else                     /* positions 4k+3 and 4k   */
        return (seq_t)n - 2;
}
