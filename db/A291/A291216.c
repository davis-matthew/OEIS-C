/*
 * A291216  Padovan numbers that are also squares.
 * https://oeis.org/A291216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, hard
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Searches Padovan numbers and keeps those that are perfect squares. */
/* a(1)=0, a(2)=1, a(3)=4, a(4)=9, a(5)=16, a(6)=49, ... */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

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

static seq_t *sqs   = NULL;          /* array of square Padovan numbers     */
static size_t sqs_cap = 0;           /* current capacity of the array      */
static size_t sqs_len = 0;           /* number of terms stored             */

static seq_t pad_a = 1, pad_b = 1, pad_c = 1; /* Padovan window P(k‑3),P(k‑2),P(k‑1)   */
static bool initialized = false;                /* state initialisation flag          */

seq_t a(idx_t n)
{
    /* n is 1‑based; the function guarantees a(n) for n≥1. */
    if (!initialized)
    {
        /* store 0 as the first term of the sequence */
        ensure_capacity(1, &sqs_cap, OEISC_VPP(&sqs), sizeof(seq_t));
        sqs[0] = 0;
        sqs_len = 1;

        /* 1 is a Padovan number and a perfect square. */
        seq_t first = pad_a; /* pad_a == 1 */
        seq_t s = floor_sqrt(first);
        if (s * s == first)
        {
            ensure_capacity(sqs_len + 1, &sqs_cap, OEISC_VPP(&sqs), sizeof(seq_t));
            sqs[sqs_len++] = first;
        }

        initialized = true;
    }

    /* Generate Padovan numbers until we have at least n square terms. */
    while (sqs_len < (size_t)n)
    {
        /* Next Padovan number: P(k) = P(k‑3) + P(k‑2)  (pad_a + pad_b) */
        seq_t next = pad_a + pad_b;

        /* Shift the sliding window for the next iteration. */
        pad_a = pad_b;
        pad_b = pad_c;
        pad_c = next;

        /* Test whether the new number is a perfect square. */
        seq_t s = floor_sqrt(next);
        if (s * s == next && next != sqs[sqs_len - 1])
        {
            ensure_capacity(sqs_len + 1, &sqs_cap, OEISC_VPP(&sqs), sizeof(seq_t));
            sqs[sqs_len++] = next;
        }
    }

    return sqs[n - 1];
}
