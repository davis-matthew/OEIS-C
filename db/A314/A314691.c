/*
 * A314691  Coordination sequence Gal.6.256.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n > 0, a(n) = 28 * (n / 6) + base[n % 6] + ( ((n /
 *            6) % 2) ? extra[n % 6] : 0 ) where base = {0,5,9,13,17,22} and
 *            extra = {0,1,2,2,2,1}
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t k = n / 6;          /* quotient */
    idx_t r = n % 6;          /* remainder */

    /* constant tables, cast to seq_t to avoid
       integer promotion issues for very small seq_t types   */
    static const seq_t base[6]  = {(seq_t)0, (seq_t)5, (seq_t)9, (seq_t)13, (seq_t)17, (seq_t)22};
    static const seq_t extra[6] = {(seq_t)0, (seq_t)1, (seq_t)2, (seq_t)2, (seq_t)2, (seq_t)1};

    seq_t result = (seq_t)28 * k + base[r];
    if (k & 1u)
        result += extra[r];

    return result;
}
