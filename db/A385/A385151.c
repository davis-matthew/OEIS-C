/*
 * A385151  a(n) is the least possible difference between the largest and
 *          smallest volumes of distinct three-cuboid combination filling an n
 *          X n X n cube.
 * https://oeis.org/A385151
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the minimal defect (difference between the largest and
 *            smallest volumes) achievable by partitioning an n×n×n cube into
 *            three rectangular cuboids of distinct dimensions. The minimum is
 *            the smaller of (i) the defect obtained by slicing the cube into
 *            three slabs (cuts all parallel to the sam...
 * Range:     u8 n <= 15, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;   /* Undefined for n < 3; return 0 */

    /* Use a 64‑bit type for all intermediate calculations */
    unsigned long long N   = (unsigned long long)n;
    unsigned long long N2  = N * N;
    unsigned long long best = N * N * N;   /* Upper bound on defect */

    /* Case 1: three parallel cuts (if possible) */
    if (n >= 6) {
        unsigned long long d = (n % 3 == 0) ? 2ULL : 3ULL;
        unsigned long long defect = N2 * d;
        if (defect < best)
            best = defect;
    }

    /* Case 2: nested cuts */
    for (idx_t a = 1; a < n; ++a) {
        unsigned long long A   = (unsigned long long)a;
        unsigned long long V3   = N2 * (unsigned long long)(n - a);

        for (idx_t b = 1; b < n; ++b) {
            unsigned long long B = (unsigned long long)b;

            unsigned long long V1 = N * A * B;
            unsigned long long V2 = N * A * ((unsigned long long)(n - b));

            /* All volumes must be distinct */
            if (V1 == V2 || V1 == V3 || V2 == V3)
                continue;

            unsigned long long maxV = V1;
            if (V2 > maxV) maxV = V2;
            if (V3 > maxV) maxV = V3;

            unsigned long long minV = V1;
            if (V2 < minV) minV = V2;
            if (V3 < minV) minV = V3;

            unsigned long long defect = maxV - minV;
            if (defect < best)
                best = defect;
        }
    }

    return (seq_t)best;
}
