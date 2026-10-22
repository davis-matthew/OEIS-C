/*
 * A354582  Number of distinct contiguous constant subsequences (or partial
 *          runs) in the k-th composition in standard order.
 * https://oeis.org/A354582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = number of distinct contiguous constant subsequences
 *            (partial runs) in the n-th composition in standard order. The
 *            algorithm generates the composition from the binary
 *            representation of n, finds for each distinct part value the
 *            maximum run length, and sums those maxima
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;       /* a(0) = 0 */

    /* For idx_t 64-bit, there are at most 64 bits set.
       Use fixed-size arrays. */
    uint64_t posList[65];
    size_t m = 0;

    /* Extract positions (1‑based) of set bits in n, traversing from LSB. */
    uint64_t tmp = n;
    for (uint64_t pos = 1; tmp; ++pos, tmp >>= 1) {
        if (tmp & 1ULL)
            posList[m++] = pos;
    }

    /* Compute first differences of the augmented position list [0]∪posList. */
    uint64_t diff[65];
    for (size_t i = 0; i < m; ++i) {
        diff[i] = posList[i] - (i == 0 ? 0 : posList[i - 1]);
    }

    /* Reverse the difference list to obtain the composition parts. */
    uint64_t comp[65];
    for (size_t i = 0; i < m; ++i)
        comp[i] = diff[m - 1 - i];

    /* Find maximum run length for each distinct part value. */
    uint64_t maxrun[65];
    for (size_t i = 0; i < 65; ++i)
        maxrun[i] = 0;

    size_t idx = 0;
    while (idx < m) {
        uint64_t val = comp[idx];
        size_t j = idx + 1;
        while (j < m && comp[j] == val)
            ++j;
        size_t runLen = j - idx;
        if (runLen > maxrun[val])
            maxrun[val] = runLen;
        idx = j;
    }

    /* Sum the maxima to get the result. */
    seq_t total = (seq_t)0;
    for (size_t v = 1; v < 65; ++v)
        if (maxrun[v])
            total += (seq_t)maxrun[v];

    return total;
}
