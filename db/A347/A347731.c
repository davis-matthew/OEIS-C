/*
 * A347731  Number of compositions (ordered partitions) of n into at most 3
 *          triangular numbers.
 * https://oeis.org/A347731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered compositions of n into at most 3
 *            triangular numbers
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n = 0 the empty composition is counted as one. */

#include <stdlib.h>

/* Helper: binary search on the pre‑computed triangular table */
static int is_triangular(idx_t x, idx_t *tri, idx_t tlen)
{
    idx_t lo = 0, hi = tlen;
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        if (tri[mid] == x) return 1;
        if (tri[mid] < x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return 0;
}

seq_t a(idx_t n)
{
    /* The empty composition of 0 */
    if (n == 0)
        return 1;

    /* Count triangular numbers up to n */
    size_t len = 0;
    for (idx_t i = 1; ; ++i) {
        __int128 t = (__int128)i * (i + 1) / 2;
        if (t > (__int128)n)
            break;
        if (t > (__int128)(UINT64_MAX))
            break;
        ++len;
    }

    /* Allocate and fill the triangular numbers array */
    idx_t *tri = (idx_t *)malloc(len * sizeof(idx_t));
    if (!tri)
        return 0;  /* allocation failure – return 0 as safe fallback */

    for (idx_t i = 1, k = 0; ; ++i) {
        __int128 t = (__int128)i * (i + 1) / 2;
        if (t > (__int128)n)
            break;
        tri[k++] = (idx_t)t;
    }

    seq_t count = 0;

    /* Single part compositions (length = 1) */
    if (is_triangular(n, tri, (idx_t)len))
        count += 1;

    /* Two part compositions (length = 2) */
    for (size_t i = 0; i < len; ++i) {
        idx_t rem = n - tri[i];
        if (rem < tri[0])  /* tri[0] is the smallest triangular number (1) */
            continue;
        if (is_triangular(rem, tri, (idx_t)len))
            count += 1;
    }

    /* Three part compositions (length = 3) */
    for (size_t i = 0; i < len; ++i) {
        for (size_t j = 0; j < len; ++j) {
            __int128 sum = (__int128)tri[i] + tri[j];
            if (sum > (__int128)n)
                continue;
            idx_t rem = n - (idx_t)sum;
            if (rem < tri[0])
                continue;
            if (is_triangular(rem, tri, (idx_t)len))
                count += 1;
        }
    }

    free(tri);
    return count;
}
