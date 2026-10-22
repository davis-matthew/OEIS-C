/*
 * A364168  Numbers that can be written in more than one way in the form
 *          (j+2k)^2-(j+k)^2-j^2 with j,k>0.
 * https://oeis.org/A364168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th smallest integer >0 that can be written in the
 *            form * (j+2k)^2-(j+k)^2-j^2 for at least two distinct pairs
 *            (j,k) with j,k>0. * The function computes it by exhaustive
 *            search over j,k up to a dynamic * limit, collecting all values
 *            that occur at least twice, sorting them and * return...
 * Range:     u8 none, u16 none, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#ifdef __cplusplus
#include <vector>
#include <algorithm>
#endif

/* global state ----------------------------------------------------------- */
static seq_t *terms = NULL;          /* sorted list of terms */
static size_t terms_size = 0;        /* number of terms stored */
static size_t terms_cap  = 0;        /* capacity of terms array */
static idx_t current_limit = 0;      /* last used bound for j,k */

/* compare function for qsort (used only in C mode) ------------------------ */
static int seq_cmp(const void *p1, const void *p2)
{
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

/* ensure terms array has enough capacity ----------------------------------- */
static void grow_terms(size_t need)
{
    if (terms_cap < need) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 128;
        if (new_cap < need) new_cap = need;
        terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        terms_cap = new_cap;
    }
}

/* regenerate the list of terms with j,k <= limit -------------------------- */
static void generate_terms(idx_t limit)
{
    /* free previous data */
    if (terms) { free(terms); terms = NULL; }
    terms_size = 0;
    terms_cap  = 0;

#ifdef __cplusplus
    /* C++ mode: use std::vector ------------------------------------------ */
    std::vector<seq_t> vals;
    vals.reserve((size_t)limit * (size_t)limit + 10);
    for (idx_t j = 1; j <= limit; ++j) {
        seq_t jj = (seq_t)j * (seq_t)j;
        for (idx_t k = 1; k <= limit; ++k) {
            seq_t kk = (seq_t)k * (seq_t)k;
            seq_t hk = (seq_t)j * (seq_t)k;
            seq_t h  = 2 * hk + 3 * kk - jj;
            if (h > 0) vals.push_back(h);
        }
    }
    std::sort(vals.begin(), vals.end());
    /* collect values that occur at least twice --------------------------- */
    size_t i = 0;
    while (i < vals.size()) {
        size_t j = i + 1;
        while (j < vals.size() && vals[j] == vals[i]) ++j;
        if (j - i >= 2) {
            grow_terms(terms_size + 1);
            terms[terms_size++] = vals[i];
        }
        i = j;
    }
#else
    /* C mode: dynamic array ---------------------------------------------- */
    seq_t *vals = NULL;
    size_t vals_cap = 0, vals_used = 0;
    for (idx_t j = 1; j <= limit; ++j) {
        seq_t jj = (seq_t)j * (seq_t)j;
        for (idx_t k = 1; k <= limit; ++k) {
            seq_t kk = (seq_t)k * (seq_t)k;
            seq_t hk = (seq_t)j * (seq_t)k;
            seq_t h  = 2 * hk + 3 * kk - jj;
            if (h > 0) {
                if (vals_used == vals_cap) {
                    size_t new_cap = vals_cap ? vals_cap * 2 : 256;
                    vals = (seq_t *)realloc(vals, new_cap * sizeof(seq_t));
                    vals_cap = new_cap;
                }
                vals[vals_used++] = h;
            }
        }
    }
    /* sort the values */
    qsort(vals, vals_used, sizeof(seq_t), seq_cmp);
    /* collect values that occur at least twice --------------------------- */
    size_t i = 0;
    while (i < vals_used) {
        size_t j = i + 1;
        while (j < vals_used && vals[j] == vals[i]) ++j;
        if (j - i >= 2) {
            grow_terms(terms_size + 1);
            terms[terms_size++] = vals[i];
        }
        i = j;
    }
    free(vals);
#endif
    current_limit = limit;
}

/* public API -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (current_limit == 0) {
        /* initial generation with a modest bound */
        generate_terms(200);
    }
    /* enlarge the search if we do not yet have enough terms */
    while (n > terms_size) {
        idx_t new_limit = current_limit * 2;
        if (new_limit == current_limit) ++new_limit; /* safety */
        generate_terms(new_limit);
    }
    /* terms are 1‑based: a(1) is the first element */
    return terms[n - 1];
}
