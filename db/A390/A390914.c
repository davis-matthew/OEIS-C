/*
 * A390914  a(n) is the maximum number of divisors (A000005) among all unique,
 *          nonzero numbers formed by permuting subsets of the digits of n.
 * https://oeis.org/A390914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   divisor_count_u64_simple, get_digits_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A390914: a(n) = maximum τ(m) over all distinct, non‑zero numbers m
   that can be formed by permuting any non‑empty subset of the decimal digits
   of n. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/divisor_count_u64_simple.h"
#include "../utils/get_digits_u64.h"

/* next_permutation for an array of ints, size n. */
static bool next_perm(int *arr, size_t n)
{
    if (n < 2) return false;
    int i = (int)n - 2;
    while (i >= 0 && arr[i] >= arr[i + 1]) i--;
    if (i < 0) return false;
    int j = (int)n - 1;
    while (arr[j] <= arr[i]) j--;
    int tmp = arr[i];
    arr[i] = arr[j];
    arr[j] = tmp;
    /* reverse suffix */
    int l = i + 1, r = (int)n - 1;
    while (l < r) {
        tmp = arr[l];
        arr[l] = arr[r];
        arr[r] = tmp;
        l++; r--;
    }
    return true;
}

/* comparator for qsort on uint64_t */
static int cmp_uint64(const void *a, const void *b)
{
    const uint64_t *ua = (const uint64_t *)a;
    const uint64_t *ub = (const uint64_t *)b;
    if (*ua < *ub) return -1;
    if (*ua > *ub) return 1;
    return 0;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* digits of n, most significant first */
    int digits[64];
    size_t dlen;
    get_digits_u64((uint64_t)n, digits, &dlen);

    /* temporary storage for numbers (max small for n ≤ 10000) */
    const size_t MAX_NUMS = 1024;
    uint64_t nums[MAX_NUMS];
    size_t num_len = 0;

    /* enumerate all non-empty subsets of digits */
    uint64_t mask_limit = ((uint64_t)1 << dlen);
    for (uint64_t mask = 1; mask < mask_limit; ++mask) {
        /* build subset digits */
        int subset[64];
        size_t sublen = 0;
        for (size_t k = 0; k < dlen; ++k)
            if (mask & ((uint64_t)1 << k))
                subset[sublen++] = digits[k];

        /* sort subset ascending to start permutation enumeration */
        for (size_t i = 0; i < sublen; ++i)
            for (size_t j = i + 1; j < sublen; ++j)
                if (subset[i] > subset[j]) {
                    int t = subset[i];
                    subset[i] = subset[j];
                    subset[j] = t;
                }

        /* enumerate all distinct permutations of the subset */
        do {
            uint64_t val = 0;
            for (size_t j = 0; j < sublen; ++j)
                val = val * 10 + (uint64_t)subset[j];
            if (val == 0) continue;   /* skip number 0 */
            if (num_len < MAX_NUMS)
                nums[num_len++] = val;
        } while (next_perm(subset, sublen));
    }

    if (num_len == 0)
        return (seq_t)0;   /* should not happen for n >= 1 */

    /* sort and deduplicate */
    qsort(nums, num_len, sizeof(uint64_t), cmp_uint64);
    size_t uniq_len = 1;
    for (size_t i = 1; i < num_len; ++i)
        if (nums[i] != nums[uniq_len - 1])
            nums[uniq_len++] = nums[i];

    /* find maximum divisor count among distinct numbers */
    seq_t best = 0;
    for (size_t i = 0; i < uniq_len; ++i) {
        seq_t dcnt = divisor_count_u64_simple(nums[i]);
        if (dcnt > best)
            best = dcnt;
    }
    return best;
}
