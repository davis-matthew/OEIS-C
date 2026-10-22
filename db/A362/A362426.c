/*
 * A362426  Number of compositions (ordered partitions) of n into 2 perfect
 *          powers (A001597).
 * https://oeis.org/A362426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered pairs of perfect powers (k^e, e ≥ 2, k
 *            ≥ 1) whose sum is n (1 is counted as 1^e). The value is returned
 *            as a seq_t
 * Range:     u8 n <= 63, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* n < 2 cannot be expressed as a sum of two positive perfect powers */
    if (n < 2)
        return 0;

    size_t max = (size_t)n;

    /* boolean array marking which numbers ≤ n are perfect powers */
    bool *is_pp = (bool*)calloc(max + 1, sizeof(bool));
    if (!is_pp)
        return 0;

    /* array to hold the list of perfect powers found (size_t is sufficient
       because n ≤ max follows immediately). */
    size_t *list = (size_t*)malloc((max + 1) * sizeof(size_t));
    if (!list) {
        free(is_pp);
        return 0;
    }
    size_t list_sz = 0;

    /* 1 is a perfect power (1^e) */
    if (max >= 1) {
        is_pp[1] = true;
        list[list_sz++] = 1;
    }

    /* generate all distinct perfect powers ≤ n */
    for (idx_t base = 2; (seq_t)base * (seq_t)base <= (seq_t)n; ++base) {
        seq_t val = (seq_t)base * (seq_t)base;
        while (val <= (seq_t)n) {
            size_t val_sz = (size_t)val;
            if (!is_pp[val_sz]) {
                is_pp[val_sz] = true;
                list[list_sz++] = val_sz;
            }
            val *= (seq_t)base;
        }
    }

    /* count ordered compositions (p, q) with p + q = n and p, q perfect powers */
    seq_t count = 0;
    for (size_t i = 0; i < list_sz; ++i) {
        size_t p = list[i];
        size_t q = n - p;
        if (q >= 1 && is_pp[q])
            count += (seq_t)1;
    }

    free(is_pp);
    free(list);
    return count;
}
