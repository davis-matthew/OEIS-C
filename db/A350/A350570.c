/*
 * A350570  a(n) is the smallest multiple of 7 that can be formed by
 *          concatenating the first n positive integers in some order.
 * https://oeis.org/A350570
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest multiple of 7 that can be formed by
 *            concatenating the first n positive integers in some order
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   cmp_numeric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/cmp_numeric.h"

/* small helper: number of decimal digits of an unsigned integer */
static idx_t digit_count(idx_t x)
{
    idx_t d = 0;
    do { d++; x /= 10; } while (x);
    return d;
}

/* comparator for minimal-lexicographic ordering of two integers
   by concatenation: a before b if a followed by b < b followed by a */
static int cmp_min_order(const void *p1, const void *p2)
{
    idx_t a = *(const idx_t *)p1;
    idx_t b = *(const idx_t *)p2;
    static seq_t pow10[6] = {1,10,100,1000,10000,100000};
    idx_t lenA = digit_count(a);
    idx_t lenB = digit_count(b);
    seq_t v1 = (seq_t)a * pow10[lenB] + (seq_t)b;
    seq_t v2 = (seq_t)b * pow10[lenA] + (seq_t)a;
    if (v1 < v2) return -1;
    if (v1 > v2) return 1;
    return 0;
}

/* next_permutation for an array of idx_t, using numeric order */
static bool next_permutation(idx_t *a, idx_t n)
{
    if (n < 2) return false;
    int i = (int)n - 2;
    while (i >= 0 && a[i] >= a[i+1]) i--;
    if (i < 0) return false;
    int j = (int)n - 1;
    while (a[j] <= a[i]) j--;
    idx_t tmp = a[i]; a[i] = a[j]; a[j] = tmp;
    for (int l = i+1, r = (int)n-1; l < r; l++, r--) {
        tmp = a[l]; a[l] = a[r]; a[r] = tmp;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;                         /* undefined below offset */

    /* pre‑compute powers of ten (seq_t) and 10^k mod 7 (int) */
    static seq_t pow10_seq[6];
    static int  pow10_mod[6];
    static bool init = false;
    if (!init) {
        pow10_seq[0] = (seq_t)1;
        pow10_mod[0]  = 1 % 7;
        for (int i = 1; i < 6; ++i) {
            pow10_seq[i] = pow10_seq[i-1] * (seq_t)10;
            pow10_mod[i] = (pow10_mod[i-1] * 10) % 7;
        }
        init = true;
    }

    /* build minimal‑lexicographic order of 1..n */
    idx_t *order = (idx_t *)malloc(sizeof(idx_t) * n);
    if (!order) return 0;
    for (idx_t i = 0; i < n; ++i) order[i] = i + 1;
    qsort(order, n, sizeof(idx_t), cmp_min_order);

    /* choose small suffix to permute: up to 7 numbers (good enough for all n) */
    idx_t m = n < 7 ? n : 7;
    idx_t prefix_cnt = n - m;

    /* compute remainder and value of the prefix (first n–m numbers) */
    int rem = 0;                 /* modulo 7 remainder */
    seq_t pref_val = 0;          /* concatenated value */
    for (idx_t i = 0; i < prefix_cnt; ++i) {
        idx_t x = order[i];
        int len = (int)digit_count(x);
        rem = (rem * pow10_mod[len] + (int)(x % 7)) % 7;
        pref_val = pref_val * pow10_seq[len] + (seq_t)x;
    }

    /* collect suffix numbers for permutation */
    idx_t *perm = (idx_t *)malloc(sizeof(idx_t) * m);
    if (!perm) { free(order); return 0; }
    for (idx_t i = 0; i < m; ++i) perm[i] = order[prefix_cnt + i];
    /* sort numerically, the ordering for next_permutation */
    qsort(perm, m, sizeof(idx_t), cmp_numeric);

    /* enumerate all permutations of the suffix */
    seq_t best_val = 0;
    bool found = false;
    do {
        int cur_rem = rem;
        seq_t cur_val = pref_val;
        for (idx_t i = 0; i < m; ++i) {
            idx_t x = perm[i];
            int len = (int)digit_count(x);
            cur_rem = (cur_rem * pow10_mod[len] + (int)(x % 7)) % 7;
            cur_val = cur_val * pow10_seq[len] + (seq_t)x;
        }
        if (cur_rem == 0) {
            if (!found || cur_val < best_val) {
                best_val = cur_val;
                found = true;
            }
        }
    } while (next_permutation(perm, m));

    free(order);
    free(perm);
    return found ? best_val : 0;
}
