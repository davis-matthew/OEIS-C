/*
 * A391476  For n >= 1, a(n) is the sum of the number of divisors A000005(r)
 *          for all distinct, nonzero numbers r formed by permuting subsets of
 *          the digits of n.
 * https://oeis.org/A391476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum_{r∈D(n)} τ(r) where D(n) is the set of distinct,
 *            non‑zero numbers obtained by permuting all non‑empty subsets of
 *            the decimal digits of n
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/divisor_count_u64.h"

/* Local helper: next lexicographic permutation of an unsigned char array. */
static int next_permutation(unsigned char *a, int n)
{
    int i = n - 2;
    while (i >= 0 && a[i] >= a[i + 1]) --i;
    if (i < 0) return 0;
    int j = n - 1;
    while (a[j] <= a[i]) --j;
    unsigned char tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;
    /* reverse a[i+1 .. n-1] */
    int l = i + 1, r = n - 1;
    while (l < r) {
        tmp = a[l];
        a[l] = a[r];
        a[r] = tmp;
        ++l; --r;
    }
    return 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* not part of the sequence but safe */

    /* Extract digits of n (least significant first). */
    unsigned char digits[20];
    int nd = 0;
    uint64_t tmp = (uint64_t)n;
    if (tmp == 0) { digits[nd++] = 0; }
    while (tmp > 0) {
        digits[nd++] = (unsigned char)(tmp % 10);
        tmp /= 10;
    }

    /* Store the distinct numbers obtained. */
    uint64_t uniq[400];
    int uniqcnt = 0;

    int maxmask = 1 << nd;
    unsigned char subset[20];

    for (int mask = 1; mask < maxmask; ++mask) {
        int k = 0;
        for (int idx = 0; idx < nd; ++idx)
            if (mask & (1 << idx))
                subset[k++] = digits[idx];

        /* sort subset to prepare for next_permutation */
        for (int i = 1; i < k; ++i) {
            unsigned char key = subset[i];
            int j = i - 1;
            while (j >= 0 && subset[j] > key) {
                subset[j + 1] = subset[j];
                --j;
            }
            subset[j + 1] = key;
        }

        /* enumerate all permutations of the subset */
        do {
            uint64_t val = 0;
            for (int j = k - 1; j >= 0; --j) val = val * 10 + subset[j];
            if (val == 0) continue;

            /* check uniqueness */
            int found = 0;
            for (int i = 0; i < uniqcnt; ++i)
                if (uniq[i] == val) { found = 1; break; }
            if (!found) uniq[uniqcnt++] = val;
        } while (next_permutation(subset, k));
    }

    /* sum divisors for each distinct number */
    seq_t sum = 0;
    for (int i = 0; i < uniqcnt; ++i)
        sum += (seq_t)divisor_count_u64(uniq[i]);

    return sum;
}
