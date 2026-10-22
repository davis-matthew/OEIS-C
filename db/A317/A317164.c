/*
 * A317164  a(n) = 55837783597462913 + (n-1)*13858932213216090.
 * https://oeis.org/A317164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 55837783597462913 + (n-1)*13858932213216090
 * Range:     u8 none, u16 none, u32 none, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For safety we
       return the first term if n == 0. */
    if (n == 0) {
        return (seq_t)55837783597462913ULL;
    }

    seq_t n_seq   = (seq_t)n;
    const seq_t start = (seq_t)55837783597462913ULL;
    const seq_t diff  = (seq_t)13858932213216090ULL;

    return start + (n_seq - 1) * diff;
}
