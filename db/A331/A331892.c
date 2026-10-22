/*
 * A331892  Positive numbers k such that the negabinary expansion (A039724) of
 *          -k is palindromic.
 * https://oeis.org/A331892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 296, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   push_back
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A331892: positive integers k whose negabinary
 * representation of –k is a palindrome.  The algorithm enumerates all
 * binary palindromes of length ≥ 2, interprets each as a negative
 * integer in base –2, and stores the corresponding k = –(value).  The
 * collected k-values are sorted and deduplicated; the function returns
 * the nth element for n ≥ 1. */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/push_back.h"

/* generate all palindromes of length L (L ≥ 2) and append the corresponding
 * positive k to the array *all.  Leading bit is 1. */
static void generate_length(size_t L, seq_t **all, size_t *len, size_t *cap)
{
    size_t half = (L + 1) / 2;
    size_t start = 1ULL << (half - 1);          /* first half with leading
                                                   1 */
    size_t end   = 1ULL << half;                /* exclusive */

    for (size_t h = start; h < end; ++h) {
        uint8_t bits[64] = {0};
        /* first half */
        for (size_t i = 0; i < half; ++i)
            bits[i] = (h >> (half - 1 - i)) & 1;
        /* mirror to second half */
        for (size_t i = half; i < L; ++i)
            bits[i] = bits[L - 1 - i];

        /* interpret bits as a value in base –2 using the recurrence:
         *   v' = –2 * v + b
         *   starting from v = 0.  The result is the value of the
         *   negabinary number. */
        int64_t val = 0;
        for (size_t i = 0; i < L; ++i)
            val = -2 * val + bits[i];

        if (val < 0) {                          /* –k is negative → k > 0 */
            seq_t k = (seq_t)(-val);
            push_back(all, len, cap, k);
        }
    }
}

/* comparator for qsort on seq_t */
static int seq_cmp(const void *p1, const void *p2)
{
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n <= 0)
        return 0;                               /* undefined for n ≤ 0 */

    /* static state shared between calls */
    static seq_t *all = NULL;      /* all k found (unsorted) */
    static size_t all_len = 0, all_cap = 0;
    static seq_t *sorted = NULL;    /* sorted, unique k's */
    static size_t sorted_len = 0, sorted_cap = 0;
    static size_t processed_len = 0;  /* max length processed */

    /* grow until we have enough terms */
    while (sorted_len < (size_t)n) {
        ++processed_len;
        generate_length(processed_len, &all, &all_len, &all_cap);

        /* build the sorted unique array from 'all' */
        if (sorted)
            free(sorted);
        sorted_cap = all_len;
        sorted = (seq_t *)malloc(sorted_cap * sizeof(seq_t));
        memcpy(sorted, all, all_len * sizeof(seq_t));

        qsort(sorted, all_len, sizeof(seq_t), seq_cmp);

        /* deduplicate */
        size_t uniq = 0;
        for (size_t i = 0; i < all_len; ++i) {
            if (i == 0 || sorted[i] != sorted[i - 1])
                sorted[uniq++] = sorted[i];
        }
        sorted_len = uniq;
    }

    return sorted[n - 1];
}
