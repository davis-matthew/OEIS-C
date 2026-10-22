/*
 * A357897  a(1)=1; thereafter a(n)=n+k, where k is the minimal value of k
 *          such that a(k)=n-1 and k belongs to [1, n-1], or k=0 if no such
 *          value exists.
 * https://oeis.org/A357897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 129, u16 n <= 8695, u32 n <= 8695, u64 n <= 8695,
 *            u128 n <= 8695, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A357897:
   a(1) = 1;
   for n > 1, a(n) = n + k, where
   k is the smallest index 1 ≤ k ≤ n‑1 such that a(k) = n‑1,
   otherwise k = 0 if no such index exists. */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* ------------------------------------------------------------
   Memoization tables.
   `seq`   : a(n) values, indexed a[0] is unused.
   `first` : first occurrence of a value v, i.e. smallest k with a(k)=v.
   ------------------------------------------------------------ */
static idx_t   seq_cap   = 0;   /* allocated capacity of seq   */
static seq_t * seq       = NULL;/* array of a(n)               */
static idx_t   seq_used  = 0;   /* highest n computed (0 means none) */

static idx_t   first_cap = 0;   /* capacity of first array      */
static idx_t * first     = NULL;/* array of first occurrences   */

/* Ensure seq[] has at least 'need + 1' capacity (index 0 unused).  */
static void ensure_seq_capacity(idx_t need)
{
    if (need < seq_cap) return;
    size_t new_cap = seq_cap ? (size_t)seq_cap << 1 : 64;
    if (new_cap < (size_t)(need + 1))
        new_cap = (size_t)(need + 1);
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = (idx_t)new_cap;
}

/* Ensure first[] can index up to value 'val'.  */
static void ensure_first_capacity(idx_t val)
{
    if (val < first_cap) return;
    size_t new_cap = first_cap ? (size_t)first_cap << 1 : 64;
    while (new_cap <= (size_t)val)
        new_cap <<= 1;
    idx_t *new_arr = (idx_t *)realloc(first, new_cap * sizeof(idx_t));
    /* preserve existing data, zero-initialize new part */
    if (first_cap < new_cap)
        memset(new_arr + first_cap, 0, (new_cap - first_cap) * sizeof(idx_t));
    first = new_arr;
    first_cap = (idx_t)new_cap;
}

/* ------------------------------------------------------------
   Compute the nth term of the sequence.
   ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)                /* offset is 1; undefined for 0 */
        return 0;

    /* Initialise on first call */
    if (seq_used == 0) {
        ensure_seq_capacity(1);
        seq[1] = (seq_t)1;
        seq_used = 1;

        ensure_first_capacity(1);
        first[1] = 1;           /* first occurrence of value 1 */
    }

    /* Compute up to n if necessary */
    if (n > seq_used) {
        ensure_seq_capacity(n);

        for (idx_t i = seq_used + 1; i <= n; ++i) {
            /* Find k: smallest index with a(k) = i-1 */
            idx_t target = i - 1;                 /* (n-1) in definition */
            idx_t k = 0;
            if (target < first_cap) {
                if (first[target] != 0)
                    k = first[target];
            }   /* else k remains 0 */

            /* Compute a(i) = i + k */
            seq_t a_i = (seq_t)i + (seq_t)k;
            seq[i] = a_i;

            /* Record first occurrence of a_i if none */
            idx_t a_val = (idx_t)a_i;            /* safe: a_i ≤ 2*i-1 */
            ensure_first_capacity(a_val);
            if (first[a_val] == 0)
                first[a_val] = i;
        }
        seq_used = n;
    }

    return seq[n];
}
