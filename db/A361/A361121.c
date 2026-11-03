/*
 * A361121  1 if n-th composite number A002808(n) is even, otherwise 0.
 * https://oeis.org/A361121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the nth composite number (A002808) is even, 0
 *            otherwise
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Validated: partial, pass, 94 terms through n = 94 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
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

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* offset is 1 – undefined for n = 0 */

    static seq_t *comps = NULL;      /* array of composite numbers */
    static size_t comp_cnt = 0;      /* number of composites stored */
    static size_t comp_cap = 0;      /* capacity of the array     */
    static idx_t last_checked = 3;   /* last integer tested (start at 3 → next is 4) */

    /* Grow the array until we have at least n composites */
    while (comp_cnt < n) {
        ++last_checked;                         /* next integer to test */
        if (is_prime(last_checked))              /* skip primes */
            continue;

        if (comp_cnt == comp_cap) {              /* need more space */
            size_t new_cap = comp_cap == 0 ? 4 : comp_cap << 1;
            ensure_capacity(new_cap, &comp_cap, OEISC_VPP(&comps),
                            sizeof(seq_t));
        }
        comps[comp_cnt++] = (seq_t)last_checked;  /* store composite */
    }

    seq_t composite = comps[n - 1];
    /* Return 1 if composite is even, 0 otherwise */
    return ((composite & (seq_t)1) == 0) ? (seq_t)1 : (seq_t)0;
}
