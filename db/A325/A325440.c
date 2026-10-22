/*
 * A325440  a(n) is the least number not 2*a(m)-1 or 3*a(m)-1 for any m < n.
 * https://oeis.org/A325440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 153, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A325440: a(n) is the least integer not of the form 2*a(m)-1 or 3*a(m)-1
   for any m < n.  The sequence is generated greedily.
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static seq_t *a_array = NULL;   /* a_array[1..] holds the sequence values */
static size_t a_cap   = 0;       /* current capacity of a_array */
static bool  *forbid  = NULL;   /* forbidden[ x ] == true if x = 2*a(k)-1 or 3*a(k)-1 */
static size_t forbid_cap = 0;   /* capacity of forbid array */
static idx_t cur_n        = 0;  /* highest n computed so far */
static seq_t last_val     = 0;  /* a(cur_n) */
static seq_t next_candidate = 1;/* candidate to check for the next term */

/* helper to grow an array of seq_t */
static void grow_a_array(size_t min_cap)
{
    if (a_cap >= min_cap) return;
    size_t new_cap = a_cap == 0 ? 4 : a_cap << 1;
    while (new_cap < min_cap) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(a_array, new_cap * sizeof(seq_t));
    if (!tmp) abort();  /* out of memory */
    a_array = tmp;
    a_cap = new_cap;
}

/* helper to grow the forbidden boolean array */
static void grow_forbid(size_t min_cap)
{
    if (forbid_cap >= min_cap) return;
    size_t new_cap = forbid_cap == 0 ? 4 : forbid_cap << 1;
    while (new_cap < min_cap) new_cap <<= 1;
    bool *tmp = (bool *)realloc(forbid, new_cap * sizeof(bool));
    if (!tmp) abort();
    /* initialise new entries to false */
    for (size_t i = forbid_cap; i < new_cap; ++i) tmp[i] = false;
    forbid = tmp;
    forbid_cap = new_cap;
}

/* main function: returns the nth term of the sequence (1‑based) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* not used per OEIS offset, but safe */

    while (cur_n < n) {
        /* find the next allowed candidate >= last_val+1 */
        size_t cand_i = (size_t)next_candidate;
        while (cand_i < forbid_cap && forbid[cand_i]) ++cand_i;
        next_candidate = (seq_t)cand_i;

        /* record the new term */
        seq_t val = next_candidate;
        size_t new_n = cur_n + 1;
        grow_a_array(new_n + 1);
        a_array[new_n] = val;
        cur_n = new_n;
        last_val = val;

        /* compute new forbidden values 2*val-1 and 3*val-1 */
        seq_t f1 = (seq_t)2 * val - 1;
        seq_t f2 = (seq_t)3 * val - 1;
        size_t max_f = f1 > f2 ? (size_t)f1 : (size_t)f2;
        grow_forbid(max_f + 1);
        forbid[(size_t)f1] = true;
        forbid[(size_t)f2] = true;

        /* next candidate starts after the new term */
        next_candidate = last_val + 1;
    }

    return a_array[n];
}
