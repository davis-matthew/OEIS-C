/*
 * A371236  Triangle T(n, k), n > 0, k = 1..n, read and filled in the greedy
 *          way by rows with distinct positive integers such that for any n >
 *          0, the terms in the n-th row are congruent modulo n.
 * https://oeis.org/A371236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 103, u16 n <= 8454, u32 n <= 8454, u64 n <= 8454,
 *            u128 n <= 8454, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: generate triangle rows greedily; a(n) gives nth flattened entry. */
/* This implements OEIS A371236. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static unsigned char *used = NULL;          /* bit array of numbers already used */
static size_t used_cap = 0;                 /* capacity of 'used' array          */
static seq_t *seq_cache = NULL;            /* flattened sequence values         */
static size_t seq_cache_len = 0;            /* number of elements in seq_cache   */
static size_t seq_cache_cap = 0;            /* capacity of seq_cache             */
static idx_t rows_done = 0;                 /* number of rows already generated  */

/* enlarge the 'used' array so that index 'need' is valid */
static void ensure_used_capacity(idx_t need)
{
    if ((size_t)need < used_cap)
        return;
    size_t new_cap = used_cap ? used_cap * 2 : 16;
    while (new_cap <= (size_t)need)
        new_cap <<= 1;
    unsigned char *tmp = (unsigned char *)realloc(used, new_cap * sizeof(*used));
    if (!tmp) abort();
    memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(*tmp));
    used = tmp;
    used_cap = new_cap;
}

/* enlarge the 'seq_cache' array so that index 'need' (1‑based) is valid */
static void ensure_seq_capacity(size_t need)
{
    if (need <= seq_cache_cap)
        return;
    size_t new_cap = seq_cache_cap ? seq_cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(*seq_cache));
    if (!tmp) abort();
    seq_cache = tmp;
    seq_cache_cap = new_cap;
}

/* append a value to the linear sequence cache */
static void seq_append(seq_t v)
{
    if (seq_cache_len == seq_cache_cap)
        ensure_seq_capacity(seq_cache_len + 1);
    seq_cache[seq_cache_len++] = v;
}

/* exported function */
seq_t a(idx_t n)
{
    if (n == 0)          /* safety, although OEIS offset starts at 1 */
        return (seq_t)0;

    /* generate rows until we have at least n terms */
    while (seq_cache_len < (size_t)n) {
        rows_done++;                     /* new row number (1‑based) */
        idx_t row = rows_done;

        /* find the smallest unused number to start the row */
        idx_t start = 1;
        for (;;) {
            ensure_used_capacity(start);
            if (!used[(size_t)start])
                break;
            ++start;
        }

        /* first element of the row */
        seq_t val_start = (seq_t)start;
        seq_append(val_start);
        used[(size_t)start] = 1;

        /* fill the remaining (row-1) elements, all congruent to start mod row */
        for (idx_t j = 1; j < row; ++j) {
            idx_t candidate = start + j * row;   /* same remainder */
            for (;;) {
                ensure_used_capacity(candidate);
                if (!used[(size_t)candidate])
                    break;
                candidate += row;
            }
            seq_t val = (seq_t)candidate;
            seq_append(val);
            used[(size_t)candidate] = 1;
        }
    }

    return seq_cache[(size_t)n - 1];
}
