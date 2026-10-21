/*
 * A264998  Number of partitions of n into distinct parts of the form 3^a*5^b
 *          or 2.
 * https://oeis.org/A264998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of partitions of n into distinct parts taken from
 *            the set {1, 2} ∪ {3^a * 5^b : a,b ≥ 0}
 * Range:     u8 n <= 917, u16 n <= 9961, u32 n <= 9961, u64 n <= 9961,
 *            u128 n <= 9961, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------------ */

#include <stdlib.h>
#include <stddef.h>

/* ---------- helper: dynamic array of idx_t --------------------------------- */
static void append_idx(size_t *len, size_t *cap, idx_t **arr, idx_t val) {
    if (*len >= *cap) {
        size_t new_cap = *cap ? (*cap * 2) : 16;
        *arr = (idx_t *)realloc(*arr, new_cap * sizeof(idx_t));
        *cap = new_cap;
    }
    (*arr)[*len] = val;
    (*len)++;
}

/* ---------- comparator for qsort on idx_t --------------------------------- */
static int cmp_idx_t(const void *a, const void *b) {
    idx_t ia = *(const idx_t *)a;
    idx_t ib = *(const idx_t *)b;
    return (ia < ib) ? -1 : (ia > ib) ? 1 : 0;
}

/* ---------- main sequence function --------------------------------------- */
seq_t a(idx_t n) {
    /* Build the list of allowed parts up to n */
    idx_t *parts = NULL;
    size_t parts_len = 0, parts_cap = 0;

    if (n >= 1) append_idx(&parts_len, &parts_cap, &parts, (idx_t)1);
    if (n >= 2) append_idx(&parts_len, &parts_cap, &parts, (idx_t)2);

    for (idx_t pow3 = 1; pow3 <= n; pow3 *= 3) {
        for (idx_t pow5 = 1; pow5 <= n / pow3; pow5 *= 5) {
            idx_t val = pow3 * pow5;
            if (val <= n && val != 1)          /* 1 already added */
                append_idx(&parts_len, &parts_cap, &parts, val);
        }
    }

    /* Sort the parts (ascending) – needed for DP loops */
    qsort(parts, parts_len, sizeof(idx_t), cmp_idx_t);

    /* DP array: dp[i] = number of partitions of i into distinct parts */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp) { free(parts); return (seq_t)0; }

    dp[0] = (seq_t)1;
    for (size_t i = 1; i <= (size_t)n; ++i) dp[i] = (seq_t)0;

    for (size_t pi = 0; pi < parts_len; ++pi) {
        idx_t p = parts[pi];
        for (idx_t i = n; i >= p; --i) {
            dp[(size_t)i] += dp[(size_t)(i - p)];
        }
    }

    seq_t result = dp[(size_t)n];
    free(dp);
    free(parts);
    return result;
}
