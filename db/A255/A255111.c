/*
 * A255111  Number of length n+4 0..2 arrays with at most one downstep in
 *          every n consecutive neighbor pairs.
 * https://oeis.org/A255111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^5 + 35 n^4 + 425 n^3 + 7525 n^2 + 16854 n)/120 + 30
 *            for n > 2, with a(1) = 243, a(2) = 622
 * Range:     u8 n <= 1, u16 n <= 16, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)243;
    if (n == 2) return (seq_t)622;

    /* use 128‑bit intermediate to avoid overflow on 64‑bit builds */
    unsigned __int128 N = (unsigned __int128)n;
    unsigned __int128 n2 = N * N;
    unsigned __int128 n3 = n2 * N;
    unsigned __int128 n4 = n3 * N;
    unsigned __int128 n5 = n4 * N;

    unsigned __int128 num = n5
                          + (unsigned __int128)35 * n4
                          + (unsigned __int128)425 * n3
                          + (unsigned __int128)7525 * n2
                          + (unsigned __int128)16854 * N;

    seq_t result = (seq_t)(num / 120);
    result += (seq_t)30;
    return result;
}
