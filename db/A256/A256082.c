/*
 * A256082  Non-palindromic balanced numbers in base 2.
 * https://oeis.org/A256082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 1624, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: computes OEIS sequence A256082 – non‑palindromic balanced
   binary numbers.  The sequence is generated on demand and memoised. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_palindrome.h"

static seq_t *cache = NULL;          /* stored sequence values           */
static size_t cache_len = 0;         /* number of values stored          */
static size_t cache_cap = 0;         /* allocated capacity of cache      */
static idx_t  processed_len = 0;     /* longest binary length processed */

/* push a new value into the cache, expanding capacity if necessary */
static void push_val(seq_t val)
{
    if (cache_len == cache_cap) {
        cache_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_len++] = val;
}

/* comparison routine for qsort over seq_t values */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t x = *(const seq_t *)a;
    const seq_t y = *(const seq_t *)b;
    return (x < y) ? -1 : (x > y) ? 1 : 0;
}

/* structure used to store a weighted sum and the corresponding mask  */
typedef struct {
    int64_t sum;      /* weighted sum of the selected bits                */
    unsigned mask;    /* mask of selected bits (left or right side)       */
} SumMask;

/* comparison routine for sorting SumMask array by sum */
static int cmp_sum(const void *a, const void *b)
{
    const SumMask *sa = (const SumMask *)a;
    const SumMask *sb = (const SumMask *)b;
    return (sa->sum < sb->sum) ? -1 : (sa->sum > sb->sum) ? 1 : 0;
}

/* generate all balanced, non‑palindromic binary numbers of a given length
   and append them to the global cache                     */
static void process_length(idx_t length)
{
    const int L = (int)length;
    const int left_size = L / 2;          /* number of bits on each side   */
    const bool has_center = (L % 2 == 1);
    const size_t count = 1ULL << left_size;  /* number of left/right masks */

    /* pre‑compute distances for the left side */
    int d[32];
    for (int i = 0; i < left_size; ++i)
        d[i] = (int)(L - 1) - 2 * i;

    /* build array of right sums (negative of the left-side distances) */
    SumMask *right_sums = (SumMask *)malloc(count * sizeof(SumMask));
    if (!right_sums) return;           /* allocation failed: abort silently   */

    for (unsigned mask = 0; mask < count; ++mask) {
        int64_t sum = 0;
        for (int k = 0; k < left_size; ++k)
            if (mask & (1U << k))
                sum -= (int64_t)d[left_size - 1 - k];
        right_sums[mask].sum = sum;
        right_sums[mask].mask = mask;
    }
    qsort(right_sums, count, sizeof(SumMask), cmp_sum);

    /* enumerate all left masks with leading 1 (MSB must be 1) */
    for (unsigned lmask = 0; lmask < count; ++lmask) {
        if ((lmask & 1U) == 0) continue;    /* leading zero would shorten length */

        int64_t sum_left = 0;
        for (int k = 0; k < left_size; ++k)
            if (lmask & (1U << k))
                sum_left += (int64_t)d[k];

        const int64_t target = -sum_left;

        /* binary search for right masks yielding the target sum */
        size_t lo = 0, hi = count;
        while (lo < hi) {
            size_t mid = lo + (hi - lo) / 2;
            if (right_sums[mid].sum < target) lo = mid + 1;
            else hi = mid;
        }

        while (lo < count && right_sums[lo].sum == target) {
            const unsigned rmask = right_sums[lo].mask;
            /* generate numbers for this left/right pair, including both
               possible center bits if this is an odd length */
            if (has_center) {
                for (int center = 0; center <= 1; ++center) {
                    seq_t num = 0;
                    for (int pos = 0; pos < L; ++pos) {
                        num <<= 1;
                        unsigned bit = 0;
                        if (pos < left_size)
                            bit = (lmask >> pos) & 1;
                        else if (pos == left_size)
                            bit = (unsigned)center;
                        else {
                            int k = pos - (left_size + 1);
                            bit = (rmask >> k) & 1;
                        }
                        num |= bit;
                    }
                    if (!is_palindrome(num, 2))
                        push_val(num);
                }
            } else {
                seq_t num = 0;
                for (int pos = 0; pos < L; ++pos) {
                    num <<= 1;
                    unsigned bit = 0;
                    if (pos < left_size)
                        bit = (lmask >> pos) & 1;
                    else {
                        int k = pos - left_size;
                        bit = (rmask >> k) & 1;
                    }
                    num |= bit;
                }
                if (!is_palindrome(num, 2))
                    push_val(num);
            }
            ++lo;
        }
    }

    free(right_sums);
}

/* public sequence routine --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for offset 1, return 0 as guard */

    /* generate terms until the desired index is reached */
    while (cache_len < (size_t)n) {
        ++processed_len;
        process_length(processed_len);
        qsort(cache, cache_len, sizeof(seq_t), cmp_seq);   /* keep cache sorted */
    }
    return cache[n - 1];
}
