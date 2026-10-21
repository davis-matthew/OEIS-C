/*
 * A254373  Digital roots of centered square numbers (A001844).
 * https://oeis.org/A254373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = digital_root(n^2 + (n-1)^2)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n mod 9 and (n-1) mod 9 safely in unsigned arithmetic. */
    idx_t nm9   = n % 9;          /* n mod 9 */
    idx_t nm1m9 = (n + 8) % 9;    /* (n-1) mod 9, avoid underflow */

    /* Compute (n^2 + (n-1)^2) mod 9. */
    idx_t r = (nm9 * nm9 + nm1m9 * nm1m9) % 9;

    /* Digital root: if the residue is 0, the root is 9. */
    if (r == 0)
        return (seq_t)9;
    return (seq_t)r;
}
