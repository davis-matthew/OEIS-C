/*
 * A273724  Place n equally-spaced points around a circle, labeled
 *          0,1,2,...,n-1. For each i = 0..n-1 such that 3i != i mod n, draw
 *          an (undirected) chord from i to (3i mod n). Then a(n) is the total
 *          number of distinct chords.
 * https://oeis.org/A273724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n==0, * n-1 if n>0 is odd, * n-2 if n == ±2 (mod 8),
 *            * n-3 if n == 4 (mod 8), * n-5 if n == 0 (mod 8)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if ((n & 1ULL) != 0)   /* odd */
        return (seq_t)n - 1;
    switch (n & 7ULL) {
    case 0ULL:               /* n == 0 (mod 8) */
        return (seq_t)n - 5;
    case 2ULL:               /* n == 2 (mod 8) */
    case 6ULL:               /* n == 6 (mod 8) */
        return (seq_t)n - 2;
    case 4ULL:               /* n == 4 (mod 8) */
        return (seq_t)n - 3;
    default:
        /* unreachable for even n, but keep for safety */
        return 0;
    }
}
