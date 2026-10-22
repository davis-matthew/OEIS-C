/*
 * A347485  Irregular triangle read by rows: T(n, k) is the q-multinomial
 *          coefficient defined by the k-th partition of n in
 *          Abramowitz-Stegun order, evaluated at q = 2.
 * https://oeis.org/A347485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = sum_{i=1}^{⎣n⎦} T(i, k_i), where k_i is the
 *            (n‑⎣∑_{j<i}p(j)⎦)‑th partition of i in Abramowitz‑Stegun order,
 *            and T(n, L) is the q‑multinomial coefficient at q = 2 defined by
 *            the recurrence T(n, (n)) = 1 T(n, L) = A022166(n, e) · T(n−e,
 *            L\{e}), e a part of L
 * Range:     u8 n <= 10, u16 n <= 26, u32 n <= 64, u64 n <= 193,
 *            u128 n <= 908, big any n
 * Helpers:   gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Our implementation uses the recurrence to evaluate T(n,L).  */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/gcd.h"

typedef struct {
    idx_t *parts;   /* parts of the partition (non‑increasing) */
    idx_t len;      /* number of parts */
} Partition;

/* ------------------------------------------------------------------ */
/* Generate all partitions of n in descending lexicographic order   */
/* ------------------------------------------------------------------ */
static void rec_partitions(idx_t rem, idx_t max_part, idx_t *temp,
                           size_t len, Partition **arr, size_t *cnt, size_t *cap)
{
    if (rem == 0) {
        idx_t *copy = (idx_t *)malloc(len * sizeof(idx_t));
        for (size_t i = 0; i < len; ++i)
            copy[i] = temp[i];

        if (*cnt == *cap) {
            *cap *= 2;
            *arr = (Partition *)realloc(*arr, *cap * sizeof(Partition));
        }
        (*arr)[*cnt].parts = copy;
        (*arr)[*cnt].len   = (idx_t)len;
        ++(*cnt);
        return;
    }

    for (idx_t part = (rem < max_part ? rem : max_part); part >= 1; --part) {
        temp[len] = part;
        rec_partitions(rem - part, part, temp, len + 1, arr, cnt, cap);
        if (part == 1) break; /* avoid wrap‑around on unsigned */
    }
}

static void generate_partitions(idx_t n, Partition **out_arr, size_t *out_cnt)
{
    size_t cap = 64;
    Partition *arr = (Partition *)malloc(cap * sizeof(Partition));
    idx_t temp[64];
    size_t cnt = 0;
    rec_partitions(n, n, temp, 0, &arr, &cnt, &cap);
    *out_arr = arr;
    *out_cnt = cnt;
}

/* ------------------------------------------------------------------ */
/* Gaussian binomial coefficient at q = 2, [n choose k]_2               */
/* Computed with step‑wise reduction to avoid overflow                 */
/* ------------------------------------------------------------------ */
static seq_t binom_q2(idx_t n, idx_t k)
{
    if (k == 0 || k == n) return (seq_t)1;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = pow2(n - k + i) - (seq_t)1;
        seq_t den = pow2(i) - (seq_t)1;

        seq_t g1 = gcd(num, den);
        num /= g1;
        den /= g1;

        seq_t g2 = gcd(res, den);
        res /= g2;
        den /= g2;

        res *= num;
        if (den != (seq_t)1) {
            res /= den;
        }
    }
    return res;
}

/* ------------------------------------------------------------------ */
/* Evaluate T(n,L) from the recurrence                                */
/* ------------------------------------------------------------------ */
static seq_t evaluate_T(idx_t n, const Partition *p)
{
    seq_t res = (seq_t)1;
    idx_t rem = n;
    for (idx_t i = 0; i < p->len; ++i) {
        idx_t e = p->parts[i];
        res *= binom_q2(rem, e);
        rem -= e;
    }
    return res;
}

/* ------------------------------------------------------------------ */
/* Abramowitz‑Stegun order: compare conjugate partitions              */
/* ------------------------------------------------------------------ */
static int compare_ASt(const void *va, const void *vb)
{
    const Partition *p1 = (const Partition *)va;
    const Partition *p2 = (const Partition *)vb;

    idx_t max1 = p1->parts[0];
    idx_t max2 = p2->parts[0];
    idx_t maxlen = max1 > max2 ? max1 : max2;

    for (idx_t i = 1; i <= maxlen; ++i) {
        idx_t c1 = 0, c2 = 0;
        if (i <= max1) {
            for (idx_t j = 0; j < p1->len; ++j)
                if (p1->parts[j] >= i) ++c1;
        }
        if (i <= max2) {
            for (idx_t j = 0; j < p2->len; ++j)
                if (p2->parts[j] >= i) ++c2;
        }
        if (c1 < c2) return -1;
        if (c1 > c2) return 1;
    }
    return 0;
}

/* ------------------------------------------------------------------ */
/* Public entry point                                                    */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    idx_t row = 1;
    idx_t rem = n;               /* index within the flattened triangle */

    while (true) {
        Partition *parts = NULL;
        size_t count = 0;
        generate_partitions(row, &parts, &count);

        if (rem <= count) {
            /* get the correct k‑th partition in A‑St order */
            qsort(parts, count, sizeof(Partition), compare_ASt);
            seq_t result = evaluate_T(row, &parts[rem - 1]);

            for (size_t i = 0; i < count; ++i) free(parts[i].parts);
            free(parts);
            return result;
        }

        /* move to next row */
        rem -= count;
        for (size_t i = 0; i < count; ++i) free(parts[i].parts);
        free(parts);
        ++row;
    }
}
