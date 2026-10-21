/*
 * A283681  Unique sequence with a(1)=1, a(2)=2, representing an array read by
 *          antidiagonals in which the i-th row is this sequence itself
 *          multiplied by i.
 * https://oeis.org/A283681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, nice, look
 * Formula:   not stated
 * Range:     u8 n <= 1011, u16 n <= 9971, u32 n <= 9971, u64 n <= 9971,
 *            u128 n <= 9971, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A283681
   The sequence satisfies a((n^2+n)/2)=n.
   Recursively it is defined by:
      a(1)=1, a(2)=2.
      For n>2, let d be the unique integer with
          (d-1)d/2 < n ≤ d(d+1)/2        // d is the diagonal number
      Set r = n - (d-1)d/2                // row index on this diagonal
          j = d+1 - r                     // column index on this diagonal
      Then a(n) = r * a(j).
   This follows from reading the array formed by the anti‑diagonals
   where the i‑th row is the sequence multiplied by i. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static void ensure_capacity(idx_t n,
                            seq_t **cache,
                            char **ready,
                            size_t *cap)
{
    size_t needed = (size_t)(n + 1);
    if (*cap >= needed) return;

    size_t oldcap = *cap;

    seq_t *tmp  = (seq_t *)realloc(*cache,  needed * sizeof(seq_t));
    if (!tmp) abort();

    char *rtmp  = (char  *)realloc(*ready,  needed * sizeof(char));
    if (!rtmp) abort();

    for (size_t i = oldcap; i < needed; ++i) {
        tmp[i]  = 0;
        rtmp[i] = 0;
    }

    *cache = tmp;
    *ready = rtmp;
    *cap   = needed;
}

/* Find the smallest d such that d*(d+1)/2 ≥ n */
static inline idx_t diagonal_of(idx_t n)
{
    idx_t low  = 1;
    idx_t high = 1;

    /* Exponential search for an upper bound */
    while (true) {
        __int128 tri = (__int128)high * (high + 1) / 2;
        if (tri >= n) break;
        high <<= 1;          /* high *= 2 */
    }

    /* Binary search between low and high */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low  = mid + 1;
    }
    return low;
}

/* Recursive computation with memoisation */
static seq_t compute_a(idx_t n,
                       seq_t *cache,
                       char  *ready)
{
    if (n <= 2) return (seq_t)n;                /* a(1)=1, a(2)=2 */

    if (ready[(size_t)n]) return cache[(size_t)n];

    idx_t d          = diagonal_of(n);
    idx_t triPrev    = (d - 1) * d / 2;
    idx_t r          = n - triPrev;
    idx_t j          = d + 1 - r;

    seq_t val_j = compute_a(j, cache, ready);
    seq_t res   = (seq_t)r * val_j;

    cache[(size_t)n] = res;
    ready[(size_t)n] = 1;
    return res;
}

seq_t a(idx_t n)
{
    static seq_t  *cache  = NULL;  /* cache[n] holds a(n) for n≥3 */
    static char   *ready  = NULL;  /* ready[n] == 1 if cache[n] computed */
    static size_t  cap    = 0;     /* current capacity of cache/ready */

    ensure_capacity(n, &cache, &ready, &cap);
    return compute_a(n, cache, ready);
}
