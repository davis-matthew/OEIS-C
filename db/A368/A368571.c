/*
 * A368571  Triangle read by rows where T(n,k) is the number of positive
 *          integers M which have both n and k as factor differences, 1 <= k <
 *          n.
 * https://oeis.org/A368571
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 8161, u16 n <= 8161, u32 n <= 8161, u64 n <= 8161,
 *            u128 n <= 8161, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A368571
 *  An entry of the triangle is
 *      T(n,k) = # of positive integers M which contain both n and k
 *               as factor differences.  (1 ≤ k < n)
 *  The sequence a(n) is obtained by reading the triangle row–wise with
 *      offset 2, i.e. a(2)=T(2,1), a(3)=T(3,1), a(4)=T(3,2), a(5)=T(4,1), …
 *
 *  For a given index n (≥2) we find the row and column of the triangle,
 *  then count the admissible M by iterating over divisors d of
 *  G = n² – k² with d < √G and testing
 *
 *          M = ( d² + (G/d)² – 2(n² + k²) ) / 16     (M > 0 integer)
 *
 *  The routine runs in O(√G) ≈ O(n) time which is far below the
 *  required 2 s limit for n ≤ 10000.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t floor_sqrt_u64(uint64_t x)
{
    uint64_t r = 0, bit = 1ULL << 62;     /* approximate start at 2^31 */
    while (bit > x) bit >>= 2;            /* find highest bit <= sqrt(x) */
    while (bit)
    {
        if (x >= r + bit)
        {
            x -= r + bit;
            r = (r >> 1) + bit;
        }
        else
            r >>= 1;
        bit >>= 2;
    }
    return r;
}

/* count of positive integers M for given n, k (1 ≤ k < n) */
static seq_t T(uint64_t n, uint64_t k)
{
    if (k >= n) return 0;
    uint64_t n2 = n * n;
    uint64_t k2 = k * k;
    uint64_t G = n2 - k2;
    if (G == 0) return 0;

    uint64_t sqG = floor_sqrt_u64(G);
    seq_t count = 0;
    for (uint64_t d = 1; d <= sqG; ++d)
    {
        if (G % d != 0) continue;
        if (d == sqG) continue;          /* need d < sqrt(G) */

        uint64_t e   = G / d;
        uint64_t d2  = d * d;
        uint64_t e2  = e * e;
        __int128 numer = (__int128)d2 + (__int128)e2
                         - (__int128)2 * (__int128)n2
                         - (__int128)2 * (__int128)k2;
        if (numer <= 0) continue;
        if (numer % 16 != 0) continue;
        /* M = numer / 16 is positive integer; count it once */
        count += 1;
    }
    return count;
}

seq_t a(idx_t N)
{
    if (N < 2) return 0;        /* index below offset not defined */

    uint64_t t = (uint64_t)N - 2;           /* t = N‑2 */
    uint64_t root = floor_sqrt_u64(1 + 8 * t);
    uint64_t r = (root - 1) / 2;            /* r = nRow – 2 */
    uint64_t nRow = r + 2;                  /* row number in triangle */
    uint64_t prefix = r * (r + 1) / 2;      /* entries before this row */
    uint64_t k = t - prefix + 1;            /* column within the row (1‑based) */

    return T(nRow, k);
}
