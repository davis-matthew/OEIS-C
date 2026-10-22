/*
 * A356150  a(n) is the sum of the positive integers whose binary expansion
 *          appears as a substring in the binary expansion of n or its
 *          complement.
 * https://oeis.org/A356150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   add_substrings, floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A356150
    a(n) = sum of distinct positive integers whose binary representation
    appears as a substring in the binary expansion of n or its complement
    (complement taken with respect to the length of n, leading zeros
    ignored).                                              */
#include <stdint.h>
#include <stdlib.h>
/* function floor_log2_uint64() is provided in the utils package */
#include "../utils/floor_log2_uint64.h"
#include "../utils/add_substrings.h"

/* comparator for qsort */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va < vb) ? -1 : (va > vb);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* not needed for OEIS offset but safe */

    /* number of bits of n (without leading zeros) */
    size_t len = (size_t)floor_log2_uint64((idx_t)n) + 1;

    /* mask of len bits:  (1<<len)-1, handle len==64 case */
    uint64_t mask;
    if (len == 64)
        mask = (uint64_t)(-1);    /* all ones */
    else
        mask = ((uint64_t)1 << len) - 1;

    uint64_t comp = mask ^ n; /* complement in same bit width */

    /* collect substrings from n and complement */
    uint64_t arr[5000];   /* 2 * 64*65/2 = 4160 max, so 5000 is safe */
    size_t cnt = 0;

    add_substrings((uint64_t)n, len, arr, &cnt);
    add_substrings(comp,        len, arr, &cnt);

    /* sort and deduplicate */
    if (cnt == 0) return 0;
    qsort(arr, cnt, sizeof(uint64_t), cmp_uint64);

    seq_t sum = (seq_t)arr[0];
    for (size_t i = 1; i < cnt; ++i) {
        if (arr[i] != arr[i - 1])
            sum += (seq_t)arr[i];
    }
    return sum;
}
