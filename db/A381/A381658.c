/*
 * A381658  Lexicographically earliest sequence of positive integers such that
 *          for each distinct positive integer t there is only one value of k
 *          such that t = a(n) = a(n+k) = a(n+2*k).
 * https://oeis.org/A381658
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

/*  Sequence A381658
 *  Lexicographically earliest sequence of positive integers such that
 *  for each distinct positive integer t there is only one value of k
 *  with t = a(n) = a(n + k) = a(n + 2*k).  The function builds the
 *  sequence incrementally and caches all previous terms.  It uses only
 *  integer arithmetic and standard C11 facilities. */

#include <stdlib.h>
#include <stdbool.h>

/* -------------  Data structures and utilities ------------- */

static idx_t generated_max = 0;      /* 1‑based index of last computed term   */
static seq_t *seq = NULL;            /* array of terms, 1‑based                */
static size_t seq_cap = 0;           /* current capacity of seq               */

struct ValueInfo {
    idx_t *pos;       /* sorted list of positions where this value occurs   */
    size_t len;       /* number of stored positions                        */
    size_t cap;       /* allocated capacity of pos array                   */
    bool has_triple;  /* true if this value already has a triple          */
};
static struct ValueInfo *infos = NULL; /* array indexed by value, 1‑based           */
static size_t infos_cap = 0;           /* current capacity of infos array           */

/* Ensure seq has room for at least idx positions (1‑based). */
static void ensure_seq_size(idx_t idx)
{
    if ((size_t)idx <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap : 128;
    while (new_cap < (size_t)idx)
        new_cap <<= 1;
    seq = (seq_t *)realloc(seq, (new_cap + 1) * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Ensure infos has room for at least val indices (1‑based). */
static void ensure_infos_size(seq_t val)
{
    if ((size_t)val < infos_cap)
        return;
    size_t new_cap = infos_cap ? infos_cap : 128;
    while (new_cap <= (size_t)val)
        new_cap <<= 1;
    infos = (struct ValueInfo *)realloc(infos, new_cap * sizeof(struct ValueInfo));
    /* initialize new slots */
    for (size_t i = infos_cap; i < new_cap; ++i) {
        infos[i].pos = NULL;
        infos[i].len = 0;
        infos[i].cap = 0;
        infos[i].has_triple = false;
    }
    infos_cap = new_cap;
}

/* Append a new position to the pos list of a value. */
static void add_position(seq_t val, idx_t pos)
{
    struct ValueInfo *info = &infos[(size_t)val];
    if (info->len == info->cap) {
        size_t new_cap = info->cap ? (info->cap << 1) : 8;
        info->pos = (idx_t *)realloc(info->pos, new_cap * sizeof(idx_t));
        info->cap = new_cap;
    }
    info->pos[info->len++] = pos;
}

/* Binary search for target in an ascending array a[0..len-1]. */
static bool binary_search(const idx_t *a, size_t len, idx_t target)
{
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (a[mid] < target)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (lo < len && a[lo] == target);
}

/* Check whether value val can be placed at position pos without violating
 * the triple rule.  If a triple is created, *created_k is set to that k;
 * otherwise *created_k is set to 0.  Returns false if placement is forbidden. */
static bool can_place(seq_t val, idx_t pos, idx_t *created_k)
{
    struct ValueInfo *info = &infos[(size_t)val];

    /* No previous occurrence: always allowed. */
    if (info->len == 0) {
        *created_k = 0;
        return true;
    }

    bool found_first = false;
    idx_t first_k = 0;

    for (size_t idx = 0; idx < info->len; ++idx) {
        idx_t j = info->pos[idx];
        if (pos <= j)                 /* new position must be after j */
            continue;
        idx_t i = 2 * j - pos;        /* i < j is required */
        if (i == 0 || i >= j)
            continue;
        if (binary_search(info->pos, idx, i)) {   /* triple found: i<j<pos */
            idx_t k = j - i;   /* k = pos - j = j - i */
            if (info->has_triple)          /* already a triple exists */
                return false;
            if (!found_first) { found_first = true; first_k = k; }
            else              /* a second distinct triple would appear */
                return false;
        }
    }

    *created_k = found_first ? first_k : 0;
    return true;
}

/* -----------------------  Main function  ----------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* Undefined for non-positive indices */

    /* Grow the sequence up to n. */
    while (generated_max < n) {
        idx_t next_idx = generated_max + 1;
        ensure_seq_size(next_idx);   /* make sure seq has room */

        seq_t candidate = 1;         /* start from the smallest positive integer */

        for (;;) {
            ensure_infos_size(candidate);
            idx_t k_val = 0;
            if (can_place(candidate, next_idx, &k_val)) {
                seq[next_idx] = candidate;   /* store the chosen value */
                add_position(candidate, next_idx);
                if (k_val != 0)                /* a triple just formed */
                    infos[(size_t)candidate].has_triple = true;
                break;
            }
            ++candidate;                       /* try next integer */
        }
        generated_max = next_idx;
    }

    return seq[n];
}
