/*
 * A276810  Numbers n such that A045876(n) has distinct decimal digits.
 * https://oeis.org/A276810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search-based implementation of OEIS sequence A276810: numbers n
   such that A045876(n) (sum of distinct permutations of n's digits)
   has all distinct decimal digits.  The sequence has exactly 146
   elements (the last is 991), so the function builds the entire
   list on first use. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/digit_counts.h"

/* Return true if all decimal digits of x are distinct. */
static bool has_unique_digits(const seq_t x)
{
    unsigned char cnt[10];
    digit_counts(x, cnt);          /* cnt[d] = number of times digit d appears */
    for (int i = 0; i < 10; ++i)
        if (cnt[i] > 1)
            return false;
    return true;
}

/* Compute the next lexicographic permutation of arr[0..n-1].
   Returns true if a next permutation exists, false if arr is the
   final (descending) permutation. */
static bool next_permutation(unsigned char *arr, size_t n)
{
    if (n < 2)
        return false;
    size_t i = n - 1;
    while (i > 0 && arr[i - 1] >= arr[i])
        --i;
    if (i == 0)
        return false;               /* already the last permutation */
    size_t j = n - 1;
    while (arr[j] <= arr[i - 1])
        --j;
    unsigned char tmp = arr[i - 1];
    arr[i - 1] = arr[j];
    arr[j] = tmp;
    /* reverse the tail */
    size_t l = i, r = n - 1;
    while (l < r) {
        tmp = arr[l];
        arr[l] = arr[r];
        arr[r] = tmp;
        ++l;
        --r;
    }
    return true;
}

/* Sum of all distinct permutations of the decimal digits of n.
   n is an unsigned 64‑bit integer (idx_t). */
static seq_t sum_permutations(idx_t n)
{
    unsigned char digits[20];
    size_t len = 0;
    idx_t t = n;
    if (t == 0) {
        digits[0] = 0;
        len = 1;
    }
    while (t > 0) {
        digits[len++] = (unsigned char)(t % 10);
        t /= 10;
    }
    /* Put digits into most‑significant‑first order */
    unsigned char arr[20];
    for (size_t i = 0; i < len; ++i)
        arr[i] = digits[len - 1 - i];

    /* Sort ascending to start with the first permutation */
    for (size_t i = 1; i < len; ++i) {
        unsigned char key = arr[i];
        size_t j = i;
        while (j > 0 && arr[j - 1] > key) {
            arr[j] = arr[j - 1];
            --j;
        }
        arr[j] = key;
    }

    seq_t sum = 0;
    do {
        seq_t val = 0;
        for (size_t i = 0; i < len; ++i)
            val = val * 10 + (seq_t)arr[i];
        sum += val;
    } while (next_permutation(arr, len));

    return sum;
}

/* Main sequence function: a(n) returns the nth element of A276810
   (n is 1‑based).  The sequence has 146 terms. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Build the table once */
    static seq_t terms[200] = {0};
    static size_t terms_cnt = 0;

    if (terms_cnt == 0) {
        for (idx_t cand = 1; terms_cnt < 146; ++cand) {
            seq_t s = sum_permutations(cand);
            if (has_unique_digits(s))
                terms[terms_cnt++] = (seq_t)cand;
        }
    }

    if (n <= terms_cnt)
        return terms[(size_t)n - 1];
    return 0;   /* undefined for indices beyond the last known term */
}
