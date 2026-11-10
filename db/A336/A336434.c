/*
 * A336434  Square array read by descending antidiagonals T(n,k): In the
 *          binary expansion of n, reverse the order of the bits in the same
 *          position as the active bits in A057716(k).
 * https://oeis.org/A336434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, base
 * Formula:   T(n,k) = n with the bits at the set positions of K = A057716(k)
 *            reversed in order (first set position swapped with last, etc.);
 *            array read by descending antidiagonals T(1,d), T(2,d-1), ...,
 *            T(d,1)
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Validated: standard, pass, 94 terms through n = 4479 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

static unsigned A336434_lg(uint64_t v)
{
    unsigned r = 0;
    while (v >>= 1) ++r;
    return r;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    uint64_t d = (uint64_t)((-1.0 + sqrt(1.0 + 8.0 * (double)N)) / 2.0);
    while (d > 0 && d * (d - 1) / 2 >= N) d--;
    while (d * (d + 1) / 2 < N) d++;
    uint64_t row = N - d * (d - 1) / 2;
    uint64_t col = d + 1 - row;

    /* K = col-th positive nonpower of 2 */
    uint64_t K = col;
    while (K - (A336434_lg(K) + 1) < col) K++;

    int pos[64];
    int m = 0;
    for (int b = 0; b < 64; b++)
        if ((K >> b) & 1) pos[m++] = b;

    uint64_t v = row;
    for (int i = 0; i < m / 2; i++) {
        int p = pos[i], q = pos[m - 1 - i];
        uint64_t bp = (v >> p) & 1, bq = (v >> q) & 1;
        if (bp != bq) v ^= ((uint64_t)1 << p) | ((uint64_t)1 << q);
    }
    return (seq_t)v;
}
