/*
 * A339912  Numbers k > 1 for which bigomega(k) < bigomega(k-1)/2, where
 *          bigomega gives the number of prime factors, counted with
 *          multiplicity.
 * https://oeis.org/A339912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k > 1 such that bigomega(k) <
 *            bigomega(k-1)/2
 * Range:     u8 n <= 54, u16 n <= 12312, u32 n <= 12313, u64 n <= 12313,
 *            u128 n <= 12313, big any n
 * Validated: standard, pass, 84 terms through n = 9779 (2026-11-03)
 * Helpers:   bigomega, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/bigomega.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#endif

/* Grow a dynamic array when needed.  The pointer is passed as (void **). */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size) /* elem_size: sizeof(T) */
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, new_cap);
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

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }

    static seq_t *terms = NULL;      /* array of found terms   */
    static size_t terms_cap = 0;     /* capacity of array     */
    static idx_t last_k  = 2;        /* next integer to test  */
    static idx_t found   = 0;        /* number of terms seen  */

    /* Allocate initial space for the array */
    if (terms == NULL) {
        terms_cap = 64;
        terms = mem_new(seq_t, terms_cap);
        if (!terms) return 0;            /* allocation failure */
    }

    /* Gather terms until we have at least n of them */
    while (found < n) {
        seq_t omega_k   = bigomega(last_k);
        seq_t omega_k_1 = bigomega(last_k - 1);

        /* Condition: bigomega(k) < bigomega(k-1)/2  ⇔ 2*bigomega(k) < bigomega(k-1) */
        if ((omega_k << 1) < omega_k_1) {
            /* Ensure enough space, then store the new term */
            ensure_capacity(found + 1, &terms_cap,
                            OEISC_VPP(&terms), sizeof(seq_t));
            terms[found] = (seq_t)last_k;
            ++found;
        }
        ++last_k;
    }

    return terms[n - 1];
}
