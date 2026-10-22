/*
 * A361927  Square array A(n, k) of distinct positive integers, n, k > 0, read
 *          and filled by upwards antidiagonals in the greedy way such that
 *          A(n, k) is a multiple of A(n, 1) and of A(1, k).
 * https://oeis.org/A361927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm:  Build a square array A(n,k) greedily – each entry is the
   smallest unused positive integer that is a multiple of A(n,1) and
   A(1,k).  The array is read by upward antidiagonals, producing the
   flat sequence a(n).  The function caches all generated values so
   that calls for arbitrary indices are handled in O(1) after an
   initial O(max(n)) build. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/gcd.h"

/* --- static state --------------------------------------------------- */
static seq_t *flat = NULL;          /* flattened permutation */
static size_t flat_cap = 0, flat_len = 0;

static unsigned char *used = NULL;  /* 0/1 flag array, indexed by integer value */
static size_t used_cap = 0;

static seq_t *first_row = NULL;    /* A(1,j)  1‑based indexing */
static size_t first_row_cap = 0;

static seq_t *first_col = NULL;    /* A(i,1)  1‑based indexing */
static size_t first_col_cap = 0;

static size_t current_d = 3;       /* next diagonal sum to process (start 3) */
static size_t current_i = 2;       /* current i in that diagonal          */

static seq_t smallest_unused = (seq_t)2;   /* next candidate for first row/col */

/* --- helpers -------------------------------------------------------- */
static void ensure_used(size_t n) {
    if (n >= used_cap) {
        size_t new_cap = used_cap ? used_cap : 64;
        while (new_cap <= n) new_cap <<= 1;
        unsigned char *tmp = (unsigned char *)realloc(used, new_cap);
        if (!tmp) abort();
        memset(tmp + used_cap, 0, new_cap - used_cap);
        used = tmp;
        used_cap = new_cap;
    }
}

static void ensure_flat(size_t new_cap) {
    if (new_cap > flat_cap) {
        size_t cap = flat_cap ? flat_cap : 64;
        while (cap < new_cap) cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(flat, cap * sizeof(seq_t));
        if (!tmp) abort();
        flat = tmp;
        flat_cap = cap;
    }
}

static void ensure_row(size_t idx) {
    if (idx >= first_row_cap) {
        size_t cap = first_row_cap ? first_row_cap : 64;
        while (cap <= idx) cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(first_row, cap * sizeof(seq_t));
        if (!tmp) abort();
        memset(tmp + first_row_cap, 0, (cap - first_row_cap) * sizeof(seq_t));
        first_row = tmp;
        first_row_cap = cap;
    }
}

static void ensure_col(size_t idx) {
    if (idx >= first_col_cap) {
        size_t cap = first_col_cap ? first_col_cap : 64;
        while (cap <= idx) cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(first_col, cap * sizeof(seq_t));
        if (!tmp) abort();
        memset(tmp + first_col_cap, 0, (cap - first_col_cap) * sizeof(seq_t));
        first_col = tmp;
        first_col_cap = cap;
    }
}

static seq_t next_unused(void) {
    while (true) {
        size_t v = (size_t)smallest_unused;
        ensure_used(v);
        if (!used[v]) {
            used[v] = 1;
            seq_t ret = smallest_unused;
            smallest_unused = smallest_unused + (seq_t)1;
            return ret;
        }
        smallest_unused = smallest_unused + (seq_t)1;
    }
}

/* --- initialization ----------------------------------------------- */
static void init(void) {
    /* flat array */
    flat_cap = 64;
    flat = (seq_t *)malloc(flat_cap * sizeof(seq_t));
    if (!flat) abort();
    flat[0] = (seq_t)1;
    flat_len = 1;

    /* used flags */
    used_cap = 64;
    used = (unsigned char *)calloc(used_cap, sizeof(unsigned char));
    if (!used) abort();
    used[(size_t)1] = 1;

    /* first row and first column */
    first_row_cap = 64;
    first_row = (seq_t *)calloc(first_row_cap, sizeof(seq_t));
    if (!first_row) abort();
    first_row[1] = (seq_t)1;   /* A(1,1) */

    first_col_cap = 64;
    first_col = (seq_t *)calloc(first_col_cap, sizeof(seq_t));
    if (!first_col) abort();
    first_col[1] = (seq_t)1;   /* A(1,1) */

    /* diagonal traversal state */
    current_d = 3;  /* start from diagonal 3 (since diagonal 2 is (1,1)) */
    current_i = 2;  /* start with i=2 in that diagonal */
}

/* --- main function ------------------------------------------------- */
seq_t a(idx_t n) {
    if (n < (idx_t)1) abort();
    if (!flat) init();

    while (flat_len < n) {
        size_t i = current_i;
        size_t j = current_d - i;   /* j >= 1 */

        seq_t val;

        /* obtain value for A(i,j) */
        if (i == (size_t)1) {
            ensure_row(j);
            if (first_row[j] == (seq_t)0) {
                first_row[j] = next_unused();
            }
            val = first_row[j];
        } else if (j == (size_t)1) {
            ensure_col(i);
            if (first_col[i] == (seq_t)0) {
                first_col[i] = next_unused();
            }
            val = first_col[i];
        } else {
            ensure_col(i);
            ensure_row(j);
            seq_t rowbase = first_col[i];
            seq_t colbase = first_row[j];
            seq_t g = gcd(rowbase, colbase);
            seq_t lcm_part = rowbase / g;
            seq_t lcm_val = lcm_part * colbase;

            seq_t candidate = lcm_val;
            while (true) {
                size_t idx = (size_t)candidate;
                ensure_used(idx);
                if (!used[idx]) break;
                candidate = candidate + lcm_val;
            }
            val = candidate;
        }

        /* mark used and store */
        ensure_used((size_t)val);
        used[(size_t)val] = 1;
        ensure_flat(flat_len + 1);
        flat[flat_len++] = val;

        /* advance to next cell in the current diagonal */
        if (current_i == (size_t)1) {
            current_d++;
            current_i = current_d - 1;
        } else {
            current_i--;
        }
    }

    return flat[n - 1];
}
