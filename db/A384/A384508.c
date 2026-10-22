/*
 * A384508  Nonnegative integers k such that the digits of k include the
 *          digits of the digital sum of k as a (not necessarily contiguous)
 *          subsequence.
 * https://oeis.org/A384508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth non‑negative integer k (starting at 0) such that the
 *            decimal digits of k contain the decimal digits of the sum of k's
 *            digits as a subsequence
 * Range:     u8 n <= 40, u16 n <= 7143, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"

#define DIGIT_BUF_MAX 256

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_count = 0;
static seq_t next_candidate = 0;

static bool is_subsequence(seq_t k, seq_t sum)
{
    unsigned char digs_k_rev[DIGIT_BUF_MAX];
    unsigned char digs_sum_rev[DIGIT_BUF_MAX];
    int len_k = 0, len_sum = 0;

    seq_t t = k;
    if (t == 0) {
        digs_k_rev[0] = 0;
        len_k = 1;
    } else {
        while (t != 0) {
            digs_k_rev[len_k++] = (unsigned char)(t % 10);
            t = t / 10;
        }
    }

    t = sum;
    if (t == 0) {
        digs_sum_rev[0] = 0;
        len_sum = 1;
    } else {
        while (t != 0) {
            digs_sum_rev[len_sum++] = (unsigned char)(t % 10);
            t = t / 10;
        }
    }

    int i = len_k - 1;
    int j = len_sum - 1;
    while (i >= 0 && j >= 0) {
        if (digs_k_rev[i] == digs_sum_rev[j])
            --j;
        --i;
    }
    return j < 0;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* safety: index 0 is undefined for this sequence */

    while (cache_count < n) {
        seq_t k = next_candidate;
        next_candidate = next_candidate + 1;

        seq_t s = digit_sum(k);
        if (is_subsequence(k, s)) {
            if (cache_count == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    exit(EXIT_FAILURE);   /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_count++] = k;
        }
    }
    return cache[n - 1];
}
