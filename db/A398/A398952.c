/*
 * A398952  Smaller members in pairs of successive 5-rough numbers that are
 *          both squarefree numbers.
 * https://oeis.org/A398952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smaller member of each successive pair of 5‑rough
 *            squarefree numbers. The sequence consists of the first element
 *            of every consecutive pair {p_k , p_{k+1}} of integers that are
 *            not divisible by 2 or 3, where both p_k and p_{k+1} are
 *            squarefree. a(1)=1 (pair {1,5}). Implementation keeps a st...
 * Range:     u8 n <= 16, u16 n <= 3812, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9309 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdint.h>
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

/* find the smallest 5‑rough integer > cur  (not divisible by 2 or 3) */
static uint64_t next_5rough(uint64_t cur)
{
    for (;;) {
        ++cur;
        if ((cur & 1) && (cur % 3)) return cur;
    }
}

static seq_t *seq_cache = NULL;     /* sequence values */
static size_t seq_len  = 0;          /* number of terms stored   */
static size_t seq_cap  = 0;          /* allocated capacity       */
static uint64_t next_first = 1;      /* next first value of a pair */

/* return true iff x is squarefree */
static bool is_squarefree(seq_t x)
{
    if (x <= (seq_t)1) return true;   /* 1 is squarefree */
    /* Quick elimination if divisible by 4 */
    if ((x & (seq_t)3) == 0 && (x % (seq_t)4) == 0) return false;
    seq_t sqrtx = floor_sqrt(x);
    uint64_t limit = (uint64_t)sqrtx;
    for (uint64_t i = 2; i <= limit; ++i) {
        seq_t sq = (seq_t)(i * i);
        if (sq > x) break;
        if ((x % sq) == 0) return false;
    }
    return true;
}

/* grow the sequence cache if needed */
static void ensure_capacity(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_cache = mem_resize(seq_t, seq_cache, new_cap);
    seq_cap  = new_cap;
}

/* main sequence routine */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, return 0 */

    /* generate terms until we have at least n terms */
    while (seq_len < (size_t)n) {
        /* current pair: first = next_first, second = next 5‑rough */
        uint64_t second = next_5rough(next_first);
        if (is_squarefree((seq_t)next_first) &&
            is_squarefree((seq_t)second)) {
            /* add first to sequence cache */
            ensure_capacity(seq_len + 1);
            seq_cache[seq_len++] = (seq_t)next_first;
        }
        /* move to next pair */
        next_first = second;
    }
    return seq_cache[n - 1];
}
