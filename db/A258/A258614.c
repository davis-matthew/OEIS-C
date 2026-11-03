/*
 * A258614  Numbers m having with the largest square <= m a common divisor >
 *          1.
 * https://oeis.org/A258614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 6342, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9587 (2026-11-03)
 * Helpers:   gcd, ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A258614: Numbers m having with the largest square <= m a common
 * divisor > 1.  A value m belongs to the sequence iff gcd(m,
 * floor_sqrt(m)) > 1.  The function evaluates terms on demand and caches
 * them so that calls with arbitrary n are fast. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
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

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Return the n‑th term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    static size_t      sz   = 0;        /* number of terms stored          */
    static size_t      cap  = 0;        /* capacity of the array           */
    static seq_t      *arr  = NULL;     /* array of terms                  */
    static seq_t      nextm = 2;        /* next candidate integer to test  */

    while (sz < n) {
        seq_t m = nextm;
        seq_t k = floor_sqrt(m);

        if (gcd(m, k) > (seq_t)1) {
            if (sz == cap) {
                size_t new_cap = cap ? cap << 1 : 1;
                grow_seq(&arr, &cap, new_cap);
            }
            arr[sz++] = m;
        }
        ++nextm;
    }
    return arr[n - 1];
}
