/*
 * A264036  Stirling transform of A077957 (aerated powers of 2).
 * https://oeis.org/A264036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} 2^k * Stirling2(n,2k)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 14, u64 n <= 24, u128 n <= 41,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 492 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Grow a dynamic array when needed.  The pointer is passed as OEISC_VPP(.) */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, (new_cap * elem_size) / sizeof(T));
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#else
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#endif

static seq_t *a_cache = NULL;   /* memoized a(n) values            */
static size_t   a_cap   = 0;    /* current capacity of a_cache     */
static idx_t    a_len   = 0;    /* number of computed a values     */

static seq_t *prev_row = NULL; /* Stirling row for n-1            */
static size_t prev_cap  = 0;   /* capacity of prev_row            */
static idx_t  prev_len   = 0;  /* length of prev_row (n-1+1)      */

static seq_t *curr_row = NULL; /* temporary row for n             */
static size_t curr_cap  = 0;   /* capacity of curr_row            */

seq_t a(idx_t n)
{
    /* Compute values up to n if not already cached */
    if (n >= a_len) {
        /* Initialisation for n = 0 */
        if (a_len == 0) {
            ensure_capacity(1, &a_cap, OEISC_VPP(&a_cache), sizeof(seq_t));
            a_cache[0] = (seq_t)1;
            a_len = 1;

            ensure_capacity(1, &prev_cap, OEISC_VPP(&prev_row), sizeof(seq_t));
            prev_row[0] = (seq_t)1;   /* S(0,0)=1 */
            prev_len   = 1;
        }

        /* Build up to n using Stirling recurrence */
        for (idx_t m = a_len; m <= n; ++m) {
            size_t m_len = (size_t)m + 1;

            /* Allocate space for the m‑th Stirling row */
            ensure_capacity(m_len, &curr_cap, OEISC_VPP(&curr_row), sizeof(seq_t));

            /* Stirling recurrence: S(m,0)=0 for m>0 */
            curr_row[0] = (seq_t)0;
            for (idx_t k = 1; k <= m; ++k) {
                seq_t term1 = (k < prev_len) ? prev_row[k] : (seq_t)0;
                seq_t term2 = prev_row[k - 1];
                curr_row[k] = term1 * (seq_t)k + term2;
            }

            /* a(m) = sum_{k even} 2^{k/2} * S(m,k) */
            seq_t a_val = (seq_t)0;
            seq_t pow2  = (seq_t)1;          /* corresponds to 2^{k/2} for k=0,2,4,… */
            for (idx_t k = 0; k <= m; k += 2) {
                a_val += curr_row[k] * pow2;
                pow2   = pow2 * (seq_t)2;
            }

            /* Store a(m) in cache */
            ensure_capacity(m + 1, &a_cap, OEISC_VPP(&a_cache), sizeof(seq_t));
            a_cache[m] = a_val;

            /* Prepare for next iteration: swap row pointers and capacities */
            seq_t *tmp_row = prev_row;
            prev_row       = curr_row;
            curr_row       = tmp_row;

            size_t tmp_cap = prev_cap;
            prev_cap       = curr_cap;
            curr_cap       = tmp_cap;

            prev_len = (idx_t)m_len;
        }

        a_len = n + 1;
    }

    return a_cache[n];
}
