/*
 * A267185  Column 2 of the square array in A267181.
 * https://oeis.org/A267185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n/2 for even n > 2 a(n) = (n+5)/2 for odd n > 2 with
 *            a(0)=0, a(1)=1, a(2)=0
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Stores the OEIS sequence A267185 */
seq_t a(idx_t n)
{
    /* Handle the first few special cases */
    if (n == 0u) return 0;
    if (n == 1u) return 1;
    if (n == 2u) return 0;

    /* For n > 2 use parity‑based formula, avoiding overflow */
    if ((n & 1u) == 0u) {
        /* even n : n/2 */
        return (seq_t)(n >> 1);
    } else {
        /* odd n : (n+5)/2 = (n-1)/2 + 3, safe for large n */
        return (seq_t)((n - 1) >> 1) + 3;
    }
}
