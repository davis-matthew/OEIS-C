/*
 * A366829  Number of 9-step self-avoiding king's tours on an n X n board
 *          summed over all starting positions.
 * https://oeis.org/A366829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   a(n) = 3349864*n^2 - 25942968*n + 47890984 for n > 7
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 39, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* small known values for n = 0..7 (index 0 unused) */
    static const seq_t small[] = {0, 0, 0, 784, 436984, 3908376, 13530576, 30543072};

    if (n <= 7)
        return small[n];

    /* compute using 64‑bit intermediate to avoid overflow */
    unsigned long long t = (unsigned long long)3349864 * n * n;
    t -= (unsigned long long)25942968 * n;
    t += 47890984ULL;

    return (seq_t)t;
}
