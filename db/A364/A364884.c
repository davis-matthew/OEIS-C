/*
 * A364884  Triangle T(n, k), n > 0, k = 1..n, read by rows; T(1, 1) = 1, and
 *          for n > 1, k = 1..n, T(n, k) is the least unseen multiple of the
 *          k-th term of the flattened sequence .
 * https://oeis.org/A364884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = first in the row of the triangle A364884 (triangle
 *            T(n,k), k=1..n, where T(n,k) is the least unseen multiple of the
 *            k‑th term of the flattened sequence)
 * Range:     u8 n <= 169, u16 n <= 9076, u32 n <= 9076, u64 n <= 9076,
 *            u128 n <= 9076, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Grow a seq_t array to at least 'needed' capacity. */
static void ensure_seq_cap(idx_t *cap, seq_t **arr, idx_t needed)
{
    if (*cap < needed) {
        idx_t new_cap = (*cap) ? (*cap << 1) : 4;
        while (new_cap < needed) new_cap <<= 1;
        *arr = (seq_t *)realloc(*arr, (new_cap + 1) * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* Grow an unsigned char bitmap to at least 'needed' capacity. */
static void ensure_used_cap(idx_t *cap, unsigned char **used, idx_t needed)
{
    if (*cap < needed) {
        idx_t new_cap = (*cap) ? (*cap << 1) : 128;
        while (new_cap < needed) new_cap <<= 1;
        *used = (unsigned char *)realloc(*used, (new_cap + 1) * sizeof(unsigned char));
        /* zero the newly added bytes */
        memset(*used + (*cap) + 1, 0, (size_t)(new_cap - *cap));
        *cap = new_cap;
    }
}

/* --- main function ------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* out of OEIS offset, safe guard */

    /* static state retained between calls */
    static bool   inited  = false;
    static idx_t  size    = 0;            /* number of terms computed */
    static idx_t  cap_seq = 0;            /* capacity of a_vals */
    static idx_t  cap_cand = 0;           /* capacity of cand   */
    static seq_t *a_vals  = NULL;         /* a_vals[1..size]    */
    static seq_t *cand    = NULL;         /* cand[1..size]      */
    static idx_t  cur_row = 2;            /* next row to generate */
    static unsigned char *used  = NULL;    /* bitmap of used values */
    static idx_t  used_cap = 0;           /* capacity of bitmap  */

    if (!inited) {
        /* initial allocation */
        cap_seq = cap_cand = 4;
        a_vals  = (seq_t *)malloc((cap_seq + 1) * sizeof(seq_t));
        cand    = (seq_t *)malloc((cap_cand + 1) * sizeof(seq_t));

        a_vals[1] = (seq_t)1;            /* T(1,1) = 1 */
        cand[1]   = (seq_t)2;            /* next multiple of 1 */

        used_cap = 128;
        used = (unsigned char *)calloc(used_cap + 1, sizeof(unsigned char));
        used[1] = 1;                     /* 1 is now used */

        size    = 1;
        inited  = true;
    }

    /* generate rows until we have at least n terms */
    while (size < n) {
        /* ensure enough space for the new terms */
        ensure_seq_cap(&cap_seq, &a_vals, size + cur_row);
        ensure_seq_cap(&cap_cand, &cand,   size + cur_row);

        for (idx_t k = 1; k <= cur_row; ++k) {
            seq_t base = a_vals[k];
            seq_t candidate = cand[k];

            /* find the least unseen multiple of 'base' */
            for (;;) {
                idx_t cand_idx = (idx_t)candidate;
                if (cand_idx <= used_cap && used[(size_t)cand_idx]) {
                    candidate += base;          /* already used – try next */
                } else {
                    break;                     /* candidate is fresh */
                }
            }

            /* record the new term */
            ++size;
            a_vals[size] = candidate;

            /* mark this value as used */
            ensure_used_cap(&used_cap, &used, (idx_t)candidate);
            used[(size_t)candidate] = 1;

            /* prepare next candidate for this base */
            cand[k] = candidate + base;

            /* store initial candidate for the new base (a[size]) */
            cand[size] = candidate * (seq_t)2;
        }

        ++cur_row;   /* next row has one more element */
    }

    return a_vals[n];
}
