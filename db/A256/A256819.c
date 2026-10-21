/*
 * A256819  Number of length n+4 0..1 arrays with at most two downsteps in
 *          every n consecutive neighbor pairs.
 * https://oeis.org/A256819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^5 + 15 n^4 + 405 n^3 - 975 n^2 + 3794 n + 360) / 120
 *            for n >= 3 a(1) = 32, a(2) = 64
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 32;
    if (n == 2) return 64;

    /* compute with 128-bit intermediate to avoid overflow when seq_t is 64‑bit */
    unsigned __int128 N   = (unsigned __int128)n;
    unsigned __int128 n2  = N * N;
    unsigned __int128 n3  = n2 * N;
    unsigned __int128 n4  = n3 * N;
    unsigned __int128 n5  = n4 * N;

    unsigned __int128 num = n5
                           + 15 * n4
                           + 405 * n3
                           - 975 * n2
                           + 3794 * N
                           + 360;

    return (seq_t)(num / 120);
}
