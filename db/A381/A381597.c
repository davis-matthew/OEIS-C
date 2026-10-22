/*
 * A381597  Lexicographically earliest sequence of positive integers such that
 *          for any t and k, with k>=1, where t = a(n) = a(n+k) = a(n+2*k),
 *          only one occurrence of k, for a given t, appears anywhere in the
 *          sequence.
 * https://oeis.org/A381597
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

/*  Lexicographically earliest sequence of positive integers such that
 *  for any value t and step k≥1, if t = a(n) = a(n+k) = a(n+2k)
 *  then k can appear only once for that t anywhere in the sequence.
 *  This implementation memoizes the sequence up to the largest index
 *  requested and uses dynamic arrays to track positions and already
 *  used steps for each distinct value.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* ------------------------------------------------------------------ */
/* Helper data structure: information for each distinct value seen.  */
/* ------------------------------------------------------------------ */
typedef struct {
    /* Positions where this value has appeared. */
    idx_t  *pos;      /* array of indices (1‑based) */
    idx_t   pos_cap;  /* capacity of pos array */
    idx_t   pos_len;  /* current number of stored positions */

    /* Steps k that have already produced a triple of this value. */
    idx_t  *steps;    /* array of step sizes */
    idx_t   step_cap; /* capacity of steps array */
    idx_t   step_len; /* current number of stored steps */
} val_info;

/* ------------------------------------------------------------------ */
/* Global state: sequence cache and per‑value information.           */
/* ------------------------------------------------------------------ */
static seq_t  *seq      = NULL; /* 1‑based sequence array */
static size_t  seq_len  = 0;     /* highest index computed so far   */
static size_t  seq_cap  = 0;     /* current capacity of seq array  */

static val_info  *vals   = NULL; /* array indexed by (value–1)     */
static size_t     vals_cap = 0;   /* capacity of vals array         */

static const idx_t INIT_CAP = 4;

/* ------------------------------------------------------------------ */
/* Utility functions                                                      */
/* ------------------------------------------------------------------ */

/* Ensure seq array can hold at least indices up to 'needed'. */
static void ensure_seq_cap(idx_t needed)
{
    if ((idx_t)seq_cap >= needed)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : INIT_CAP;
    while ((idx_t)new_cap < needed)
        new_cap <<= 1;
    seq = (seq_t *)realloc(seq, (new_cap + 1) * sizeof(seq_t)); /* +1 for 1‑based */
    seq_cap = new_cap;
}

/* Grow the positions array inside a val_info structure. */
static void grow_pos(val_info *v)
{
    if ((idx_t)v->pos_cap <= v->pos_len) {
        size_t new_cap = v->pos_cap ? v->pos_cap * 2 : INIT_CAP;
        v->pos = (idx_t *)realloc(v->pos, new_cap * sizeof(idx_t));
        v->pos_cap = new_cap;
    }
}

/* Grow the steps array inside a val_info structure. */
static void grow_step(val_info *v)
{
    if ((idx_t)v->step_cap <= v->step_len) {
        size_t new_cap = v->step_cap ? v->step_cap * 2 : INIT_CAP;
        v->steps = (idx_t *)realloc(v->steps, new_cap * sizeof(idx_t));
        v->step_cap = new_cap;
    }
}

/* Ensure vals array can index value 'val'.
   Return pointer to the corresponding val_info. */
static val_info *get_val_info(seq_t val)
{
    idx_t index = (idx_t)val - 1;  /* values start from 1 */
    if ((idx_t)vals_cap <= index) {
        size_t new_cap = vals_cap ? vals_cap * 2 : INIT_CAP;
        while ((idx_t)new_cap <= index)
            new_cap <<= 1;
        vals = (val_info *)realloc(vals, new_cap * sizeof(val_info));
        /* initialise new entries */
        for (idx_t i = vals_cap; i < new_cap; ++i) {
            vals[i].pos       = NULL;
            vals[i].pos_cap   = 0;
            vals[i].pos_len   = 0;
            vals[i].steps     = NULL;
            vals[i].step_cap  = 0;
            vals[i].step_len  = 0;
        }
        vals_cap = new_cap;
    }
    return &vals[index];
}

/* Check whether step 'd' has already been used for this value. */
static bool step_used(const val_info *v, idx_t d)
{
    for (idx_t i = 0; i < v->step_len; ++i)
        if (v->steps[i] == d)
            return true;
    return false;
}

/* Record a new step 'd' for this value. */
static void add_step(val_info *v, idx_t d)
{
    if (!step_used(v, d)) {
        grow_step(v);
        v->steps[v->step_len++] = d;
    }
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                              */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)         /* undefined; offset starts at 1 */
        return (seq_t)0;

    /* Compute sequence up to position n if necessary */
    if (seq_len < (size_t)n) {

        for (idx_t pos = (idx_t)seq_len + 1; pos <= n; ++pos) {

            /* Find the smallest admissible candidate for this position */
            for (seq_t cand = 1; ; ++cand) {

                val_info *v = get_val_info(cand);
                bool ok = true;

                /* Check whether candidate would create a forbidden triple */
                for (idx_t i = 0; i < v->pos_len; ++i) {
                    idx_t j = v->pos[i];
                    idx_t d = pos - j;                /* step from j to pos */
                    /* compute left = j - d; guard against underflow */
                    int64_t left = (int64_t)j - (int64_t)d;
                    if (left >= 1 && seq[(idx_t)left] == cand) {
                        if (step_used(v, d)) {
                            ok = false;
                            break;
                        }
                    }
                }
                if (!ok)
                    continue;  /* try next candidate */

                /* Accept the candidate */
                ensure_seq_cap(pos);
                seq[pos] = cand;

                /* Record this new position */
                grow_pos(v);
                v->pos[v->pos_len++] = pos;

                /* Record any new steps that are now completed */
                for (idx_t i = 0; i < v->pos_len - 1; ++i) {
                    idx_t j = v->pos[i];
                    idx_t d = pos - j;
                    int64_t left = (int64_t)j - (int64_t)d;
                    if (left >= 1 && seq[(idx_t)left] == cand)
                        add_step(v, d);
                }
                break; /* move to next position */
            }
        }
        seq_len = (size_t)n;
    }
    return seq[n];
}
