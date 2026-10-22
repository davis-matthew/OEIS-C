/*
 * A315724  Coordination sequence Gal.6.665.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = 6*n if n mod 6 ∉ {2,4} a(n) = 6*n -
 *            3*ceil((n-2)/6) = 6*n - 3*floor((n+3)/6) if n mod 6 ∈ {2,4} (n
 *            is the index, a(n) is the n‑th term of A315724)
 * Range:     u8 n <= 42, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)                     /* special case for the offset */
        return (seq_t)1;

    seq_t val = 6 * (seq_t)n;        /* 6*n */

    /* if n mod 6 is 2 or 4, apply the correction term */
    if ((n % 6u) == 2u || (n % 6u) == 4u)
    {
        /* ceil((n-2)/6) == floor((n+3)/6) for integer n */
        seq_t k = (seq_t)( (n + 3u) / 6u );  /* integer division */
        val -= 3 * k;
    }

    return val;
}
