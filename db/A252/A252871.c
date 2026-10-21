/*
 * A252871  Number of nX3 nonnegative integer arrays with upper left 0 and
 *          lower right n+3-4 and value increasing by 0 or 1 with every step
 *          right or down.
 * https://oeis.org/A252871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^6 + 18*n^5 + 100*n^4 + 120*n^3 + 79*n^2 - 678*n + 720)
 *            / 360
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 104, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    unsigned __int128 N128 = (unsigned __int128)N;

    unsigned __int128 n2 = N128 * N128;
    unsigned __int128 n3 = n2 * N128;
    unsigned __int128 n4 = n3 * N128;
    unsigned __int128 n5 = n4 * N128;
    unsigned __int128 n6 = n5 * N128;

    unsigned __int128 num = n6;
    num += (unsigned __int128)18 * n5;
    num += (unsigned __int128)100 * n4;
    num += (unsigned __int128)120 * n3;
    num += (unsigned __int128)79 * n2;
    num += (unsigned __int128)720;
    num -= (unsigned __int128)678 * N128;

    seq_t result = (seq_t)(num / 360ULL);
    return result;
}
