/*
 * A375709  Numbers k such that A013929(k+1) = A013929(k) + 1. In other words,
 *          the k-th nonsquarefree number is 1 less than the next
 *          nonsquarefree number.
 * https://oeis.org/A375709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: A375709
 * Indices k such that the k‑th and (k+1)-st nonsquarefree integers differ by 1.
 * Computed by incremental generation of nonsquarefree numbers and recording the
 * indices where consecutive nonsquarefrees differ by 1.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    /* offset starts at 1; return 0 for n == 0 as a safe default */
    if (n == 0) return (seq_t)0;

    /* static state for incremental generation */
    static seq_t *a_arr = NULL;      /* array of computed a(k) */
    static size_t a_len = 0;         /* number of a(k) stored so far */
    static size_t a_cap = 0;         /* array capacity */

    static seq_t m_scan = 4;         /* next number to test (first nonsquarefree) */
    static idx_t nsf_idx = 0;        /* count of nonsquarefrees seen so far */
    static seq_t last_prev_nsf = 0;  /* previous nonsquarefree value */
    static idx_t last_prev_idx = 0;  /* its index (nsf_idx-1) */

    /* Grow a_arr if needed */
    if (a_len == a_cap) {
        size_t new_cap = a_cap ? a_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure – return 0 */
        a_arr = tmp;
        a_cap = new_cap;
    }

    /* Generate terms until we have a(n) */
    while (a_len < (size_t)n) {
        /* Check if m_scan is nonsquarefree */
        if (!is_squarefree(m_scan)) {
            ++nsf_idx;  /* its position in the nonsquarefree list */

            if (nsf_idx > 1 && m_scan - last_prev_nsf == (seq_t)1) {
                a_arr[a_len++] = (seq_t)last_prev_idx;  /* store the k value */
            }
            last_prev_nsf = m_scan;
            last_prev_idx = nsf_idx;
        }
        ++m_scan;
    }

    return a_arr[n - 1];
}
