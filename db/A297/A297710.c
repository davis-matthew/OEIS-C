/*
 * A297710  Partition numbers (A000041) in which parity of digits alternates.
 * https://oeis.org/A297710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   alternate_parity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Function: a(n)
   Returns the n-th partition number whose decimal digits have
   alternating parity (even, odd, even, … or odd, even, odd, …).
   Indexed with offset 1 (a(1) = 1).  The routine builds partition
   numbers on demand and keeps those that satisfy the digit test.  */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alternate_parity.h"

/* Ensure array 'arr' has capacity at least 'need' */
static void ensure_cap(seq_t **arr, size_t *cap, size_t need)
{
    if (need <= *cap)
        return;
    size_t new_cap = (*cap) ? (*cap) : 1;
    while (new_cap < need)
        new_cap <<= 1;
#ifdef __cplusplus
    seq_t *tmp = new seq_t[new_cap];
#else
    seq_t *tmp = malloc(new_cap * sizeof(seq_t));
#endif
    if (*arr) {
        for (size_t i = 0; i < *cap; ++i)
            tmp[i] = (*arr)[i];
#ifdef __cplusplus
        delete[] *arr;
#else
        free(*arr);
#endif
    }
    *arr   = tmp;
    *cap   = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for the offset 1 sequence */

    /* static storage for the partition table and the accepted list */
    static seq_t *part_arr   = NULL;
    static size_t part_cap   = 0;
    static idx_t  part_len   = 0;    /* next m to compute */

    static seq_t *found_arr  = NULL;
    static size_t found_cap  = 0;
    static idx_t  found_len  = 0;    /* number of accepted values */

    /* initialise partition table with p(0)=1 */
    if (!part_arr) {
        ensure_cap(&part_arr, &part_cap, 1);
        part_arr[0] = 1;
        part_len    = 1;
    }

    /* grow accepted list until we have n items */
    while (found_len < n) {
        /* ensure we have room for p(part_len) */
        ensure_cap(&part_arr, &part_cap, part_len + 1);

        /* compute p(part_len) by the pentagonal recurrence */
        idx_t m   = part_len;
        seq_t sum = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > m) break;
            seq_t term = part_arr[m - g1];
            if (k & 1)
                sum += term;
            else
                sum -= term;
            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 <= m) {
                term = part_arr[m - g2];
                if (k & 1)
                    sum += term;
                else
                    sum -= term;
            }
        }
        part_arr[part_len] = sum;

        /* if the number has alternating parity, store it */
        if (alternate_parity(sum)) {
            ensure_cap(&found_arr, &found_cap, found_len + 1);
            found_arr[found_len++] = sum;
        }

        ++part_len;
    }

    return found_arr[n - 1];
}
