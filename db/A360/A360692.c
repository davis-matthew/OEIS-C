/*
 * A360692  a(0) = 0. Thereafter a(n+1) = a(a(n)) if a(n) has not occurred
 *          previously, otherwise a(n+1) = n - 1 - a(n-1).
 * https://oeis.org/A360692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 512, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A360692
   a(0) = 0
   for n ≥ 0
     if a(n) has not occurred previously
        a(n+1) = a(a(n))
     else
        a(n+1) = n - 1 - a(n-1)
 */

/* sequence arithmetic uses only integer operations.
   The implementation keeps a dynamic cache for arbitrary-precision
   builds and updates an occurrence counter to decide the recurrence.  */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* storage for previously computed terms */
static seq_t *seq_arr = NULL;   /* a[0 .. seq_len-1] */
static size_t seq_cap = 0;      /* allocated size of seq_arr   */
static size_t seq_len = 0;      /* number of terms stored      */

/* occurrence counter
   occ[v] == 0 : never seen
   occ[v] == 1 : seen once (the current term)
   occ[v] >= 2 : seen two or more times
   The size of occ must be at least max_value+1. */
static unsigned char *occ = NULL;
static size_t occ_cap = 0;

/* ensure seq_arr has capacity for at least min_cap elements */
static void ensure_seq_capacity(size_t min_cap)
{
    if (min_cap <= seq_cap) return;
    size_t cap = seq_cap ? seq_cap : 1;
    while (cap < min_cap) cap <<= 1;
    seq_t *p = (seq_t *)realloc((void *)seq_arr, cap * sizeof(seq_t));
    if (!p) abort();
    seq_arr = p;
    seq_cap = cap;
}

/* ensure occ has indexable range 0 .. min_val */
static void ensure_occ_capacity(size_t min_val)
{
    if (min_val + 1 <= occ_cap) return;
    size_t old_cap = occ_cap;
    size_t cap = occ_cap ? occ_cap : 1;
    while (cap <= min_val) cap <<= 1;
    unsigned char *p = (unsigned char *)realloc((void *)occ, cap * sizeof(unsigned char));
    if (!p) abort();
    occ = p;
    if (cap > old_cap) memset(p + old_cap, 0, cap - old_cap);
    occ_cap = cap;
}

/* return the n‑th term (0‑based).  */
seq_t a(idx_t n)
{
    /* guard against impossibly large n that would overflow malloc size_t */
    if (n >= (idx_t)SIZE_MAX) abort();

    size_t target = (size_t)n;
    while (seq_len <= target) {
        size_t k = seq_len;                 /* index of term to compute */

        /* initialise a(0) */
        if (k == 0) {
            ensure_seq_capacity(1);
            seq_arr[0] = (seq_t)0;
            ensure_occ_capacity(0);
            occ[0] = 1;                     /* first occurrence of 0 */
            seq_len = 1;
            continue;
        }

        /* value of a(k-1) is known */
        seq_t prev_val = seq_arr[k-1];
        /* index into occ and seq_arr */
        size_t pv = (size_t)(idx_t)prev_val;   /* safe, prev_val ≤ k-1 */

        bool repeated = occ[pv] > 1;          /* has a(k-1) appeared before k-1 ? */

        seq_t next;
        if (!repeated) {                      /* novel term */
            /* a(k) = a(a(k-1));  a(k-1) < k-1 so array entry exists */
            size_t idx = (size_t)(idx_t)prev_val;
            next = seq_arr[idx];
        } else {                              /* repeat term */
            /* a(k) = (k-2) - a(k-2)  (valid for k ≥ 2) */
            seq_t prevprev = seq_arr[k-2];
            next = (seq_t)(k-2) - prevprev;
        }

        /* store the new term */
        ensure_seq_capacity(k + 1);
        seq_arr[k] = next;

        /* update the occurrence counter for next */
        size_t nv = (size_t)(idx_t)next;
        ensure_occ_capacity(nv);
        if (occ[nv] == 0)
            occ[nv] = 1;
        else
            occ[nv] = 2;       /* saturate at 2 */

        seq_len++;
    }

    return seq_arr[n];
}
