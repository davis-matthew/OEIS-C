/*
 * A373921  The last entry in the difference table for {the n-th row of
 *          A177028 arranged in increasing order}.
 * https://oeis.org/A373921
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 127, i16 n <= 9655, i32 n <= 9655, i64 n <= 9655,
 *            i128 n <= 9655, big any n
 * Helpers:   isqrt128_unsigned
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/isqrt128_unsigned.h"

/* Comparator for qsort on seq_t values. */
static int cmp_seq_t(const void *a, const void *b) {
    seq_t aa = *(const seq_t *)a;
    seq_t bb = *(const seq_t *)b;
    return (aa < bb) ? -1 : (aa > bb);
}

/* Last entry in the difference table for the sorted A177028 row. */
seq_t a(idx_t n) {
    if (n < 3) return 0;   /* value below the offset is irrelevant */

    /* Number of potential k values is at most n-2 (k=3..n). */
    size_t cap = (size_t)n - 2;
    seq_t *arr = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!arr) return 0;     /* allocation failure */

    size_t count = 0;
    uint64_t n64 = (uint64_t)n;

    for (uint64_t k = 3; k <= n64; ++k) {
        __int128 km4 = (__int128)k - 4;        /* k-4 (may be negative) */
        __int128 km2 = (__int128)k - 2;        /* k-2 (positive) */
        __int128 D = km4 * km4 + (__int128)8 * n64 * km2;   /* discriminant */

        if (D < 0) continue;                  /* should not occur */

        uint64_t s = isqrt128_unsigned((unsigned __int128)D);
        if ((unsigned __int128)s * s != (unsigned __int128)D) continue;   /* not a square */

        /* m = (k-4 + s) / (2*(k-2)) */
        __int128 numerator   = km4 + (__int128)s;
        __int128 denominator = (__int128)2 * km2;
        if (denominator == 0) continue;               /* k cannot be 2 */

        if (numerator % denominator != 0) continue;    /* m not integer */

        __int128 m = numerator / denominator;
        if (m <= 0) continue;                         /* m must be positive */

        /* Store k as a term of the row. */
        if (count < cap)
            arr[count++] = (seq_t)k;
    }

    if (count == 0) {                           /* should not happen for n ≥ 3 */
        free(arr);
        return 0;
    }

    /* Sort the terms increasingly. */
    qsort(arr, count, sizeof(seq_t), cmp_seq_t);

    /* Build the difference table until one element remains. */
    while (count > 1) {
        for (size_t i = 0; i + 1 < count; ++i)
            arr[i] = arr[i + 1] - arr[i];
        --count;
    }

    seq_t result = arr[0];
    free(arr);
    return result;
}
