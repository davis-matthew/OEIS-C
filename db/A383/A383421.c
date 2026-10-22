/*
 * A383421  a(1)=1; thereafter if a(n-1) is a first occurrence, then a(n) is
 *          the number of values that occur exactly once in the sequence thus
 *          far. Otherwise; a(n) is the number of terms that are the same
 *          distance away from an earlier equal value as a(n-1) is from its
 *          previous last occurrence.
 * https://oeis.org/A383421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A383421
 * a(1) = 1;
 * For n > 1:
 *   Let v = a(n-1).
 *   If v occurs exactly once in a(1..n-1) (i.e. first occurrence so far), then
 *     a(n) = number of values that occur exactly once in a(1..n-1).
 *   Else
 *     Let d = (n-1) – previous last occurrence of v.
 *     a(n) = number of terms i (≤ n-1) such that there exists j < i
 *            with a[i] = a[j] and i-j = d.  (The later index of each
 *            equal‑value pair with distance d is counted.) */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* External linkage required by the harness */
seq_t a(idx_t n);

/* Internal data structures -------------------------------------------------- */
typedef struct {
    seq_t   value;      /* the sequence value */
    size_t  count;      /* number of occurrences so far */
    idx_t   last;       /* index of last occurrence */
    idx_t   prev;       /* index of previous occurrence (before last) */
    idx_t  *indices;    /* list of all occurrence indices for this value */
    size_t  idx_used;   /* number of indices stored */
    size_t  idx_cap;    /* capacity of indices list */
} ValueInfo;

/* Global cached state -------------------------------------------------------- */
static idx_t        current_max = 0;   /* highest index computed so far   */
static seq_t       *seq_vals   = NULL; /* a[1 .. current_max]           */
static size_t       seq_cap    = 0;    /* capacity of seq_vals array      */

static ValueInfo   *infos      = NULL; /* array of distinct value infos   */
static size_t       infos_cap  = 0;
static size_t       infos_used = 0;

static size_t      *dist_counts = NULL; /* dist_counts[d] = number of pairs with later index i and distance d */
static size_t       dist_cap    = 0;

static size_t      single_occurrence = 0; /* number of values with frequency exactly one */

/* Utility functions ---------------------------------------------------------- */
static void
ensure_seq_capacity(idx_t needed)
{
    if (needed <= seq_cap) return;
    idx_t new_cap = seq_cap ? seq_cap : 256;
    while (new_cap <= needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc((void *)seq_vals, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq_vals = tmp;
    seq_cap  = new_cap;
}

static void
ensure_dist_capacity(idx_t needed)
{
    if (needed <= dist_cap) return;
    idx_t new_cap = dist_cap ? dist_cap : 256;
    while (new_cap <= needed) new_cap <<= 1;
    size_t *tmp = (size_t *)realloc((void *)dist_counts, new_cap * sizeof(size_t));
    if (!tmp) abort();
    /* initialise new slots to zero */
    for (idx_t i = dist_cap; i < new_cap; ++i) tmp[i] = 0;
    dist_counts = tmp;
    dist_cap    = new_cap;
}

static ValueInfo *
find_value(const seq_t val)
{
    for (size_t i = 0; i < infos_used; ++i)
        if (infos[i].value == val) return &infos[i];
    return NULL;
}

static ValueInfo *
ensure_value(const seq_t val)
{
    ValueInfo *v = find_value(val);
    if (v) return v;
    /* create new entry */
    if (infos_used == infos_cap) {
        size_t new_cap = infos_cap ? infos_cap * 2 : 256;
        ValueInfo *tmp = (ValueInfo *)realloc((void *)infos, new_cap * sizeof(ValueInfo));
        if (!tmp) abort();
        /* initialise new slots */
        for (size_t i = infos_used; i < new_cap; ++i) {
            tmp[i].value = 0;
            tmp[i].count = 0;
            tmp[i].last  = 0;
            tmp[i].prev  = 0;
            tmp[i].indices = NULL;
            tmp[i].idx_used = 0;
            tmp[i].idx_cap  = 0;
        }
        infos = tmp;
        infos_cap = new_cap;
    }
    ValueInfo *newInfo = &infos[infos_used++];
    newInfo->value = val;
    newInfo->count = 0;
    newInfo->last  = 0;
    newInfo->prev  = 0;
    newInfo->indices = NULL;
    newInfo->idx_used = 0;
    newInfo->idx_cap  = 0;
    return newInfo;
}

static void
ensure_indices_capacity(ValueInfo *info, size_t needed)
{
    if (needed <= info->idx_cap) return;
    size_t new_cap = info->idx_cap ? info->idx_cap : 8;
    while (new_cap < needed) new_cap <<= 1;
    idx_t *tmp = (idx_t *)realloc((void *)info->indices, new_cap * sizeof(idx_t));
    if (!tmp) abort();
    info->indices = tmp;
    info->idx_cap = new_cap;
}

/* Main sequence function ----------------------------------------------------- */
seq_t
a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, return 0 for safety */

    /* Initialise on first call */
    if (current_max == 0) {
        seq_cap = 256;
        seq_vals = (seq_t *)calloc(seq_cap, sizeof(seq_t));
        if (!seq_vals) abort();

        dist_cap = 256;
        dist_counts = (size_t *)calloc(dist_cap, sizeof(size_t));
        if (!dist_counts) abort();

        infos_cap = 256;
        infos = (ValueInfo *)calloc(infos_cap, sizeof(ValueInfo));
        if (!infos) abort();
        infos_used = 0;
        single_occurrence = 0;
    }

    /* Compute terms up to needed index */
    if (n > current_max) {
        ensure_seq_capacity(n);
        for (idx_t i = current_max + 1; i <= n; ++i) {
            if (i == 1) {
                seq_vals[1] = (seq_t)1;
                ValueInfo *info1 = ensure_value((seq_t)1);
                /* Record first occurrence */
                ensure_indices_capacity(info1, 1);
                info1->indices[0] = 1;
                info1->idx_used    = 1;
                info1->count       = 1;
                info1->last        = 1;
                info1->prev        = 0;
                single_occurrence = 1;
                continue;
            }

            /* Determine next value using recurrence */
            seq_t prev_val = seq_vals[i - 1];
            ValueInfo *prev_info = find_value(prev_val);
            if (!prev_info) abort();          /* should always exist */
            seq_t new_val;
            if (prev_info->count == 1) {
                /* first occurrence of prev_val so far */
                new_val = (seq_t)single_occurrence;
            } else {
                idx_t d = (i - 1) - prev_info->prev;
                if (d >= dist_cap) ensure_dist_capacity(d);
                new_val = (seq_t)dist_counts[d];
            }

            /* Insert new_val at position i */
            ValueInfo *new_info = find_value(new_val);
            if (!new_info) new_info = ensure_value(new_val);

            /* Update distance counts: for each earlier occurrence j of new_val */
            for (size_t k = 0; k < new_info->idx_used; ++k) {
                idx_t j = new_info->indices[k];
                idx_t d = i - j;
                if (d >= dist_cap) ensure_dist_capacity(d);
                dist_counts[d]++;
            }

            /* Append current index to indices list */
            ensure_indices_capacity(new_info, new_info->idx_used + 1);
            new_info->indices[new_info->idx_used++] = i;

            /* Update frequency and single_occurrence counter */
            size_t old_count = new_info->count;
            new_info->count = old_count + 1;
            if (old_count == 0) {
                single_occurrence++;
            } else if (old_count == 1) {
                single_occurrence--;
            }

            /* Update last and prev occurrence indices */
            idx_t old_last = new_info->last;
            new_info->prev = old_last;
            new_info->last = i;

            /* Store the value in the sequence array */
            seq_vals[i] = new_val;
        }
        current_max = n;
    }

    return seq_vals[n];
}
