/*
 * A257627  Triangle, read by rows, T(n,k) = t(n-k, k) where t(n,m) =
 *          f(m)*t(n-1,m) + f(n)*t(n,m-1) and f(x) = 7*x + 3.
 * https://oeis.org/A257627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Triangle read by rows, T(n,k) = t(n-k, k) where t(0,0)=1,
 *            t(n,m)=0 if n<0 or m<0, else t(n,m)=f(m)*t(n-1,m)+f(n)*t(n,m-1)
 *            with f(x)=7*x+3. a(n) is the flattened triangle with a(0)=1
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 37, u64 n <= 95, u128 n <= 262,
 *            big any n
 * Helpers:   free_seq_ptr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/free_seq_ptr.h"

/* ---------- helper for multiplication ---------- */
#ifndef __cplusplus
/* In C we use 128‑bit arithmetic to avoid overflow until the
   result is truncated to seq_t. */
static inline seq_t mul_seq(seq_t a, seq_t b)
{
    return (seq_t)((unsigned __int128)a * (unsigned __int128)b);
}
#else
/* In C++ the operator * works for any seq_t that supports multiplication. */
static inline seq_t mul_seq(seq_t a, seq_t b)
{
    return a * b;
}
#endif

/* ---------- helpers for dynamic allocation ---------- */
static inline seq_t *alloc_seq(size_t n)
{
#ifdef __cplusplus
    return new seq_t[n];
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}

/* ---------- function to append a value to a dynamic array ---------- */
static void append_to_array(seq_t **arr, size_t *len, size_t *cap, seq_t value)
{
    if (*len == *cap) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        seq_t *tmp = alloc_seq(new_cap);
        if (!tmp) return;          /* allocation failed – leave untouched */
        for (size_t i = 0; i < *len; ++i) tmp[i] = (*arr)[i];
        free_seq_ptr(*arr);
        *arr = tmp;
        *cap = new_cap;
    }
    (*arr)[(*len)++] = value;
}

/* ---------- the sequence ---- */
seq_t a(idx_t n)
{
    /* static state ------------------------------------------------ */
    static seq_t *built = NULL;          /* flattened sequence */
    static size_t built_len = 0;         /* number of stored terms   */
    static size_t built_cap = 0;         /* capacity of built        */

    static seq_t *prev_row = NULL;       /* previous row of t         */
    static size_t prev_len = 0;          /* length of previous row    */

    /* build rows until we have at least n+1 terms ----------------- */
    while (built_len <= (size_t)n) {
        /* next row length */
        size_t new_len = prev_len + 1;

        /* allocate new row */
        seq_t *new_row = alloc_seq(new_len);
        if (!new_row) return 0;          /* allocation failed (unlikely) */

        if (new_len == 1) {              /* first row only has (0,0)=1 */
            new_row[0] = 1;
        } else {
            /* compute row element by element */
            for (size_t k = 0; k < new_len; ++k) {
                if (k == 0) {
                    /* new_row[0] = f(0)*prev_row[0]  where f(0)=3 */
                    new_row[0] = mul_seq((seq_t)3, prev_row[0]);
                } else if (k == new_len - 1) {
                    /* last element: f(0)*prev_row[new_len-2] */
                    new_row[k] = mul_seq((seq_t)3, prev_row[k - 1]);
                } else {
                    /* middle elements */
                    seq_t fk = mul_seq((seq_t)7, (seq_t)k) + (seq_t)3;                       /* f(k)   */
                    seq_t frowk = mul_seq((seq_t)7,
                                          (seq_t)(new_len - 1 - k)) + (seq_t)3;                /* f(r-k) */
                    new_row[k] = mul_seq(fk,     prev_row[k]) +
                                 mul_seq(frowk, prev_row[k - 1]);
                }
            }
        }

        /* append the new row to the flattened array */
        for (size_t i = 0; i < new_len; ++i)
            append_to_array(&built, &built_len, &built_cap, new_row[i]);

        /* free old previous row and set new row as previous */
        free_seq_ptr(prev_row);
        prev_row = new_row;
        prev_len = new_len;
    }

    /* at this point built_len > n, so built[n] is valid */
    return built[n];
}
