/*
 * A337063  a(n) = 1 for n < 2; a(n) = freq(a(n-1),n) * freq(a(n-2),n) for n
 *          >= 2, where freq(i,j) is the number of times i appears in
 *          [a(0),a(1),...,a(j-1)].
 * https://oeis.org/A337063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = freq(a(n-1),n) * freq(a(n-2),n) a(0) = 1, a(1) = 1
 *            (freq(x,n) = number of times value x occurs in a[0 .. n-1])
 * Range:     u8 n <= 489, u16 n <= 9350, u32 n <= 9350, u64 n <= 9350,
 *            u128 n <= 9350, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Type for dictionary entries (value → frequency) */
typedef struct {
    seq_t key;
    size_t count;
} Entry;

#ifndef __cplusplus
/*--- C implementation ----------------------------------------------*/

static seq_t *a_seq      = NULL;   /* dynamic array for the sequence */
static size_t a_cap      = 0;      /* current allocation of a_seq      */
static idx_t  a_done    = 0;       /* highest index computed (0‑based)   */

static Entry *dict      = NULL;   /* dynamic array for the dictionary */
static size_t dict_cap   = 0;      /* current allocation of dict       */
static size_t dict_cnt   = 0;      /* number of entries used           */

/* Ensure enough space for a_seq */
static void ensure_a_capacity(size_t need)
{
    if (a_cap < need) {
        size_t new_cap = a_cap ? a_cap * 2 : 16;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = realloc(a_seq, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        a_seq = tmp;
        a_cap = new_cap;
    }
}

/* Ensure enough space for dict */
static void ensure_dict_capacity(size_t need)
{
    if (dict_cap < need) {
        size_t new_cap = dict_cap ? dict_cap * 2 : 16;
        if (new_cap < need) new_cap = need;
        Entry *tmp = realloc(dict, new_cap * sizeof(Entry));
        if (!tmp) abort();
        dict = tmp;
        dict_cap = new_cap;
    }
}

/* Linear search in the dictionary */
static size_t dict_find(const seq_t *k)
{
    for (size_t i = 0; i < dict_cnt; ++i)
        if (dict[i].key == *k) return i;
    return (size_t)-1;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)1;

    /* initialise once */
    if (a_done == 0) {
        ensure_a_capacity(2);
        a_seq[0] = (seq_t)1;
        a_seq[1] = (seq_t)1;
        a_done   = 1;

        ensure_dict_capacity(1);
        dict[0].key   = (seq_t)1;
        dict[0].count = 2;
        dict_cnt      = 1;
    }

    /* compute missing terms up to n */
    for (idx_t i = a_done + 1; i <= n; ++i) {
        /* frequencies of the previous two terms */
        size_t idx1 = dict_find(&(a_seq[i-1]));
        size_t idx2 = dict_find(&(a_seq[i-2]));
        size_t c1   = dict[idx1].count;
        size_t c2   = dict[idx2].count;

        seq_t val = (seq_t)c1 * (seq_t)c2;

        ensure_a_capacity((size_t)i + 1);
        a_seq[i] = val;

        /* update dictionary with the new value */
        size_t idxv = dict_find(&val);
        if (idxv != (size_t)-1)
            dict[idxv].count++;
        else {
            ensure_dict_capacity(dict_cnt + 1);
            dict[dict_cnt].key   = val;
            dict[dict_cnt].count = 1;
            dict_cnt++;
        }
    }

    a_done = n;
    return a_seq[(size_t)n];
}

#else
/*--- C++ implementation ---------------------------------------------*/

static std::vector<seq_t> a_vec;        /* sequence values */
static std::vector<Entry> dict;        /* value → frequency */

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)1;

    /* initialise once */
    if (a_vec.empty()) {
        a_vec.reserve(16);
        a_vec.push_back((seq_t)1);                    /* a(0) */
        a_vec.push_back((seq_t)1);                    /* a(1) */
        dict.push_back( Entry{ (seq_t)1, 2 } ); /* frequency of 1  */
    }

    /* compute missing terms up to n */
    for (idx_t i = a_vec.size(); i <= n; ++i) {
        /* find frequencies of the previous two terms */
        size_t idx1 = 0, idx2 = 0;
        for (size_t j = 0; j < dict.size(); ++j) {
            if (dict[j].key == a_vec[i-1]) { idx1 = j; break; }
        }
        for (size_t j = 0; j < dict.size(); ++j) {
            if (dict[j].key == a_vec[i-2]) { idx2 = j; break; }
        }
        size_t c1 = dict[idx1].count;
        size_t c2 = dict[idx2].count;

        seq_t val = (seq_t)c1 * (seq_t)c2;
        a_vec.push_back(val);

        /* update dictionary with the new value */
        bool found = false;
        for (size_t j = 0; j < dict.size(); ++j) {
            if (dict[j].key == val) { dict[j].count++; found = true; break; }
        }
        if (!found) dict.push_back( Entry{ val, 1 } );
    }

    return a_vec[(size_t)n];
}
#endif
