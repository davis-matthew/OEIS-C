/*
 * A313722  Coordination sequence Gal.6.248.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS Formula: a(n) = 5*n - (floor((n-5)/11) + 1) + (n % 11 == 6 ? 1 : 0)   (n >= 5)
   The first five values are defined directly: a(0)=1, a(1)=5, a(2)=10, a(3)=15, a(4)=20. */
seq_t a(idx_t n)
{
    /* First five terms are known explicitly. */
    static const seq_t first[5] = {1, 5, 10, 15, 20};

    if (n < 5)      /* a(0) … a(4) */
        return first[n];

    /* For n >= 5 we use the closed‑form expression.  The division is performed
       with the unsigned type idx_t, which is safe because n >= 5. */
    idx_t k = (n - 5) / 11;          /* floor((n-5)/11) */
    seq_t offset = (seq_t)k + 1;     /* floor((n-5)/11) + 1 */

    /* Reduce the offset by 1 when n ≡ 6 (mod 11). */
    if ((n % 11) == 6)
        offset -= 1;

    /* Return the result: 5*n - offset. */
    return 5 * (seq_t)n - offset;
}
