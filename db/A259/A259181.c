/*
 * A259181  a(n) = n*(n+1)*(n+2)*(n+3)*(2*n^2+6*n+7)/360.
 * https://oeis.org/A259181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(n+3)*(2*n^2+6*n+7)/360
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 94, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use unsigned __int128 for intermediate arithmetic.
       This is safe for all n up to at least 10⁴ (the intended range),
       as the largest intermediate value (~2·10²⁴) is well below 2¹²⁸. */
    unsigned __int128 prod =
        (unsigned __int128)n *
        (n + 1) *
        (n + 2) *
        (n + 3) *
        ((unsigned __int128)2 * n * n + (unsigned __int128)6 * n + 7);

    prod /= 360;          /* exact division by 360 */

    return (seq_t)prod;   /* cast back to the build‑specific integer type */
}
