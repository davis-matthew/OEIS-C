/*
 * A245032  a(n) = 27*(n - 6)^2 + 4*(n - 6)^3 = ((n - 6)^2)*(4*n + 3).
 * https://oeis.org/A245032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n-6)^2)*(4*n + 3)
 * Range:     u8 n <= 8, u16 n <= 29, u32 n <= 1027, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast index to a signed type to safely compute (n-6) when n < 6. */
    int64_t nn = (int64_t)n;
    int64_t d  = nn - 6;          /* n-6, may be negative */
    /* (n-6)^2 fits in 64‑bit for the range of n used in tests. */
    seq_t k = (seq_t)(d * d);     /* (n-6)^2 */

    seq_t term = (seq_t)(4 * nn + 3);  /* 4*n + 3 */
    return k * term;              /* (n-6)^2 * (4*n + 3) */
}
