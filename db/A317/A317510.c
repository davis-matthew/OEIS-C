/*
 * A317510  Numbers (4p+1)/3 where p is a Sophie Germain prime p > 3.
 * https://oeis.org/A317510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4p+1)/3 where p is the nth Sophie Germain prime > 3 (the
 *            sequence starts with a(1) = 7 for p = 5)
 * Range:     u8 n <= 13, u16 n <= 657, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 74 terms through n = 4817 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
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

static seq_t *sg_vals = NULL;        /* Array of sequence values */
static size_t sg_count = 0;          /* Number of values stored */
static size_t sg_capacity = 0;
static idx_t next_candidate = 5;     /* Next odd number to test (first SG prime > 3) */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* undefined but safe */

    /* Expand_sequence until we have at least n terms */
    while (sg_count < n) {
        /* Ensure we have space */
        ensure_capacity(sg_count + 1, &sg_capacity, OEISC_VPP(&sg_vals),
                        sizeof(seq_t));

        /* Find next Sophie Germain prime */
        idx_t p = next_candidate;
        while (1) {
            if (is_prime(p) && is_prime(p * 2 + 1)) {
                /* Compute (4p+1)/3 as seq_t */
                seq_t val = ((seq_t)4 * p + 1) / 3;
                sg_vals[sg_count++] = val;
                next_candidate = p + 2;   /* Next odd number after p */
                break;
            }
            p += 2;   /* Only test odd numbers */
        }
    }

    return sg_vals[n - 1];   /* 1-based indexing */
}
