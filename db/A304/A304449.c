/*
 * A304449  Numbers that are either squarefree or a perfect power.
 * https://oeis.org/A304449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 178, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A304449: Numbers that are either squarefree or a perfect power.
   The function generates the sequence by scanning the integers, testing
   membership, and memoising the results. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_squarefree_u64.h"

/* Helper: integer square root of a 64‑bit value (floor). */
static unsigned long long isqrt_u64(unsigned long long n)
{
    unsigned long long lo = 0, hi = n >> 1, mid, sq;
    if (n < 2) return n;
    while (lo <= hi) {
        mid = (lo + hi) >> 1;
        sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

/* Test whether x (uint64_t) is a perfect power (exponent ≥ 2).
   1 is considered a perfect power (1^k, k≥2). */
static bool is_perfect_power_u64(unsigned long long x)
{
    if (x <= 1) return true;        /* 1 = 1^k, k≥2 */
    unsigned long long max_base = isqrt_u64(x);
    for (unsigned long long base = 2; base <= max_base; ++base) {
        unsigned __int128 power = (unsigned __int128)base * base;
        while (power < x) power *= base;
        if (power == x) return true;
    }
    return false;
}

/* Grow the static cache to at least 'needed' elements. */
static void grow_seq(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap >= needed) return;
    size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (!*arr) abort();
    *cap = new_cap;
}

/* External linkage – required function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* defensive; offset is 1 */

    static seq_t *seq = NULL;
    static size_t seq_cap = 0;
    static size_t seq_len = 0;
    static unsigned long long candidate = 1ULL;

    while (seq_len < (size_t)n) {
        if (is_squarefree_u64(candidate) || is_perfect_power_u64(candidate)) {
            if (seq_len == seq_cap) grow_seq(&seq, &seq_cap, seq_len + 1);
            seq[seq_len++] = (seq_t)candidate;
        }
        ++candidate;
    }

    return seq[n - 1];
}
