/*
 * A371473  a(1) = 1; for n>1, if a(n-1) is squarefree, a(n) = a(n-1) + n,
 *          otherwise a(n) = squarefree kernel of a(n-1).
 * https://oeis.org/A371473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 9714, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9847 (2026-11-03)
 * Helpers:   is_squarefree, mem, radical
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A371473
   a(1) = 1
   For n > 1,
      if a(n‑1) is squarefree   →   a(n) = a(n‑1) + n
      otherwise                 →   a(n) = rad(a(n‑1))
   (rad is the square‑free kernel, i.e., product of distinct prime
   factors.) */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_squarefree.h"
#include "../utils/radical.h"
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

static seq_t *seq_cache = NULL;   /* 1‑based cache of computed terms */
static size_t seq_cap   = 0;      /* current capacity of cache */
static idx_t  seq_len   = 0;      /* largest n such that seq_cache[n] is valid */

/* Return the nth term (n ≥ 1, offset 1).  Behaviour for n = 0 is undefined. */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1; behaviour below offset is irrelevant */
        return 0;

    if (n > seq_len) {
        /* Increase cache size if necessary */
        size_t new_cap = (size_t)n + 1; /* 1‑based indexing, keep room for n */
        ensure_capacity(new_cap, &seq_cap, OEISC_VPP(&seq_cache),
                        sizeof(seq_t));

        /* Compute missing terms */
        for (idx_t i = seq_len + 1; i <= n; ++i) {
            seq_t prev = seq_cache[i - 1];
            if (is_squarefree(prev))
                seq_cache[i] = prev + (seq_t)i;
            else
                seq_cache[i] = radical((idx_t)prev);
        }
        seq_len = n;
    }

    return seq_cache[n];
}
