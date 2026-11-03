/*
 * A278328  Numbers n such that abs(n - rev(n)) is a square, where rev(n) is
 *          the decimal reverse of n (A004086).
 * https://oeis.org/A278328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 65, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 81 terms through n = 9049 (2026-11-03)
 * Helpers:   ipow_i32, mem, reverse_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A278328: numbers n such that |n - rev(n)| is a perfect square. */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/reverse_digits_idx.h"
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

seq_t a(idx_t n) {
    static seq_t *seq = NULL;          /* array of sequence terms */
    static size_t seq_len = 0;         /* number of terms stored */
    static size_t seq_cap = 0;         /* allocated capacity */
    static idx_t next_candidate = 0;   /* next integer to test */

    /* generate terms until we have at least n of them */
    while (seq_len < n) {
        idx_t cand = next_candidate;
        idx_t rev_cand = reverse_digits_idx(cand);
        idx_t diff = cand >= rev_cand ? cand - rev_cand : rev_cand - cand;
        seq_t diff_seq = (seq_t)diff;
        seq_t sq = floor_sqrt(diff_seq);

        if (sq * sq == diff_seq) {
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap == 0 ? 16 : seq_cap * 2;
                ensure_capacity(new_cap, &seq_cap, OEISC_VPP(&seq), sizeof(seq_t));
            }
            seq[seq_len++] = (seq_t)cand;
        }
        next_candidate = cand + 1;
    }

    return seq[n - 1];
}
