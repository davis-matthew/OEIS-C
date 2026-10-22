/*
 * A365230  Triangle T(n, k), n > 0, k = 1..n, read by rows and filled the
 *          greedy way with distinct positive integers such that T(n, k) is a
 *          multiple of T(k, 1).
 * https://oeis.org/A365230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 163, u16 n <= 9065, u32 n <= 9065, u64 n <= 9065,
 *            u128 n <= 9065, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A365230 sequence generator
 *
 * The sequence is the row‑major flattening of a triangle T(n,k) (n>0,
 * 1 ≤ k ≤ n) constructed greedily.
 *   • T(n,1)  = smallest unused positive integer.
 *   • T(n,k)  = smallest unused multiple of T(k,1)   for k > 1.
 *
 * This function returns the n‑th element of that flattening (offset 1).
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Ensure 'used' has room for indices up to m (inclusive). */
static void ensure_used_capacity(idx_t m, uint8_t **used, size_t *usedCap)
{
    if (m >= *usedCap) {
        size_t newCap = (*usedCap > 0) ? (*usedCap << 1) : 8;
        while (newCap <= (size_t)m) newCap <<= 1;
        uint8_t *tmp = (uint8_t *)realloc(*used, newCap);
        if (!tmp) exit(1);
        memset(tmp + *usedCap, 0, newCap - *usedCap);
        *used = tmp;
        *usedCap = newCap;
    }
}

/* Append a value to a dynamic array. */
static void append_value(seq_t val, seq_t **arr, size_t *len, size_t *cap)
{
    if (*len >= *cap) {
        size_t newCap = (*cap > 0) ? (*cap << 1) : 8;
        seq_t *tmp = (seq_t *)realloc(*arr, newCap * sizeof(seq_t));
        if (!tmp) exit(1);
        *arr = tmp;
        *cap = newCap;
    }
    (*arr)[*len] = val;
    (*len)++;
}

/* Ensure capacity for arrays indexed from 1 up to 'need' inclusive. */
static void ensure_capacity(size_t need, seq_t **arr, size_t *cap)
{
    if (need >= *cap) {
        size_t newCap = (*cap > 0) ? (*cap << 1) : 4;
        while (newCap <= need) newCap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*arr, (newCap + 1) * sizeof(seq_t));
        if (!tmp) exit(1);
        *arr = tmp;
        *cap = newCap;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* undefined for offset 1 */

    /* Static state */
    static seq_t *seq                  = NULL;  static size_t seqLen = 0;  static size_t seqCap = 0;
    static uint8_t *used               = NULL;  static size_t usedCap = 0;
    static seq_t *firstVal             = NULL;  static size_t firstCap = 0;
    static seq_t *nextMult             = NULL;  static size_t nextCap = 0;
    static idx_t  rowCount             = 0;     /* number of rows generated */

    /* Initialise on first call */
    if (!seq) {
        /* allocate first row: 1 */
        ensure_used_capacity(1, &used, &usedCap);
        used[1] = 1;

        append_value((seq_t)1, &seq, &seqLen, &seqCap);

        ensure_capacity(1, &firstVal, &firstCap);
        firstVal[1] = (seq_t)1;

        ensure_capacity(1, &nextMult, &nextCap);
        nextMult[1] = (seq_t)2;          /* next multiple of first row */

        rowCount = 1;
    }

    /* Generate rows until we have at least n terms */
    while (seqLen < (size_t)n) {
        /* Start a new row */
        rowCount++;                    /* row indices are 1‑based */

        /* Ensure indices for the new row */
        ensure_capacity(rowCount, &firstVal, &firstCap);
        ensure_capacity(rowCount, &nextMult, &nextCap);

        /* Iterate columns k=1..rowCount */
        for (idx_t k = 1; k <= rowCount; ++k) {
            seq_t base = firstVal[(size_t)k];   /* T(k,1) */
            seq_t cand = nextMult[(size_t)k];

            /* Find smallest unused multiple of 'base' */
            for (;;) {
                idx_t idx = (idx_t)cand;
                ensure_used_capacity(idx, &used, &usedCap);
                if (!used[idx]) break;
                cand += base;
            }

            seq_t val = cand;

            /* Mark used and append to sequence */
            idx_t vidx = (idx_t)val;
            ensure_used_capacity(vidx, &used, &usedCap);
            used[vidx] = 1;
            append_value(val, &seq, &seqLen, &seqCap);

            /* Update next multiple for this base */
            nextMult[(size_t)k] = cand + base;

            /* Record first element of this row */
            if (k == 1) {
                firstVal[rowCount] = val;
                nextMult[rowCount] = val + val;   /* first multiple of the new base */
            }
        }
    }

    return seq[(size_t)n - 1];
}
