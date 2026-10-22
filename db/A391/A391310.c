/*
 * A391310  Array read by ascending antidiagonals: A(n, k) = HurwitzZeta(-n,
 *          k) - HurwitzZeta(-n, k+n) with k >= 0.
 * https://oeis.org/A391310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the array A(i,j) = Sum_{k=0..i-1}
 *            (j+k)^i by ascending antidiagonals: for each d = i + j the
 *            entries (i,j) with i+j=d are enumerated with decreasing i. This
 *            function implements that reading directly from the definition
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required helper: the two types seq_t and idx_t are defined by the build.  */
#include <stdint.h>

/* Helper to compute cumulative number of entries up to and including diagonal d:
   cumulative(d) = (d+1)(d+2)/2.  The result fits in 128‑bit for all d that
   fit in 64‑bit. */
static inline uint64_t cumulative(uint64_t d)
{
    return (uint64_t)(((unsigned __int128)(d + 1) * (d + 2)) / 2);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Find the diagonal d such that cumulative(d-1) <= n < cumulative(d).
       We look for the smallest d with cumulative(d) > n. */
    uint64_t idx = (uint64_t)n;        /* n fits in uint64_t */
    uint64_t low = 0, high = 1;
    while (cumulative(high) <= idx)
        high <<= 1;                      /* exponential search for an upper bound */

    while (low < high) {
        uint64_t mid = (low + high) >> 1;
        if (cumulative(mid) > idx)
            high = mid;
        else
            low = mid + 1;
    }
    uint64_t d = low;                    /* d is the diagonal containing n */

    /* Compute offset within diagonal: index of element with i+j = d */
    uint64_t prev = (uint64_t)(((unsigned __int128)d * (d + 1)) / 2);
    uint64_t offset = idx - prev;        /* offset is k */

    uint64_t nrow = d - offset;          /* n = i (row index) */
    uint64_t kcol = offset;              /* k = j (column index) */

    /* Sum_{i=0}^{nrow-1} (kcol + i)^{nrow} */
    seq_t sum = 0;
    if (nrow > 0) {
        for (uint64_t i = 0; i < nrow; ++i) {
            seq_t base = (seq_t)(kcol + i);
            seq_t power = 1;
            for (uint64_t exp = 0; exp < nrow; ++exp)
                power *= base;
            sum += power;
        }
    }
    return sum;
}
