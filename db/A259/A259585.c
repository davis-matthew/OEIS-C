/*
 * A259585  Numbers k such that [r[s*k]] - [s[r*k]] = -1, where r = sqrt(2),
 *          s=sqrt(3), and [ ] = floor.
 * https://oeis.org/A259585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10, u32 n <= 2341, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 8872 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A259585: numbers k such that floor(sqrt(2)*floor(sqrt(3)*k)) -
   floor(sqrt(3)*floor(sqrt(2)*k)) = -1.
   The function generates the terms by an incremental brute‑force
   search and caches already found k's to answer subsequent calls
   efficiently. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

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

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

seq_t a(idx_t n) {
    static seq_t *list = NULL;
    static size_t list_len = 0;
    static size_t list_cap = 0;
    static idx_t last_k = 0;

    if (n == 0)
        return (seq_t)0;   /* undefined for n < 1, but harmless */

    while (list_len < n) {
        ++last_k;
        seq_t kk = (seq_t)last_k;

        /* a = floor(sqrt(3)*k)  -> floor_sqrt(3*k^2) */
        seq_t a = floor_sqrt((seq_t)3 * kk * kk);
        /* b = floor(sqrt(2)*k)  -> floor_sqrt(2*k^2) */
        seq_t b = floor_sqrt((seq_t)2 * kk * kk);

        /* left = floor(sqrt(2)*floor(sqrt(3)*k)) -> floor_sqrt(2*a^2) */
        seq_t left  = floor_sqrt((seq_t)2 * a * a);
        /* right = floor(sqrt(3)*floor(sqrt(2)*k)) -> floor_sqrt(3*b^2) */
        seq_t right = floor_sqrt((seq_t)3 * b * b);

        /* Condition left + 1 == right ensures the difference is -1. */
        if (left + (seq_t)1 == right)
            append_candidate(&list, &list_len, &list_cap, kk);
    }

    return list[n - 1];
}
