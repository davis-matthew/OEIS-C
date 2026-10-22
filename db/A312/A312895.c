/*
 * A312895  Coordination sequence Gal.4.58.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>=1: a(n)=4*n + |{i<=n : i≡2 (mod 7)}| + |{i<=n :
 *            i≡6 (mod 7)}|
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* That is, a(n) = 4*n + (n>=2?((n-2)/7+1):0) + (n>=6?((n-6)/7+1):0) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t cnt = 0;
    if (n >= 2) {
        cnt += (seq_t)((n - 2) / 7 + 1);
    }
    if (n >= 6) {
        cnt += (seq_t)((n - 6) / 7 + 1);
    }

    return (seq_t)4 * (seq_t)n + cnt;
}
