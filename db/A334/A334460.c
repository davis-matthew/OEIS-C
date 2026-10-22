/*
 * A334460  Irregular triangle read by rows: T(n,k) is the number of
 *          partitions of n into k consecutive parts that differ by 4, and the
 *          first element of column k is in the row that is the k-th hexagonal
 *          number (A000384).
 * https://oeis.org/A334460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = value of the irregular triangle T(n,k) of partitions of n into
   k consecutive parts differing by 4, read row‑wise (entries are 0 or 1). */
#include <stdint.h>
#include <stdbool.h>

static idx_t isqrt_u64(idx_t x) {
    idx_t res = 0;
    /* simple incremental search – fast enough for the sizes we need */
    while ((unsigned __int128)(res + 1) * (res + 1) <= x)
        ++res;
    return res;
}

seq_t a(idx_t N) {
    if (N == 0) return 0;            /* offset 1, ignored value */

    idx_t cumulative = 0;            /* number of entries produced so far */

    for (idx_t r = 1; ; ++r) {       /* iterate over rows */
        idx_t k_max = (1 + isqrt_u64(1 + 8 * r)) / 4;
        idx_t prev_cum = cumulative;
        cumulative += k_max;

        if (cumulative >= N) {       /* the wanted entry lies in this row */
            idx_t k = N - prev_cum;  /* column index, 1‑based */
            /* check existence of partition of r into k consecutive parts
               with difference 4.  The first part a must be positive: */
            idx_t numerator = r - 2 * k * (k - 1);
            return (seq_t)((numerator > 0) && (numerator % k == 0));
        }
    }
}
