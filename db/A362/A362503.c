/*
 * A362503  a(n) is the number of k such that n - A000045(k) is a square.
 * https://oeis.org/A362503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of Fibonacci numbers F(k) (with F(0)=0, F(1)=1,
 *            F(2)=1,...) such that n – F(k) is a perfect square. A000045(1)
 *            and A000045(2) (both equal 1) are counted as separate
 *            occurrences. The function computes this directly by generating
 *            Fibonacci numbers up to n and testing each difference w...
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 115 terms through n = 8883 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/ipow_i32.h"

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

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t count = 0;

    /* Fibonacci numbers start with F(0) = 0, F(1) = 1. */
    seq_t prev = 0;   /* F(0) */
    seq_t curr = 1;   /* F(1) */

    /* Count F(0) */
    if (prev <= n_seq) {
        seq_t diff = n_seq - prev;
        seq_t s = floor_sqrt(diff);
        if (s * s == diff) {
            ++count;
        }
    }

    /* Count the first F(1) (=1) */
    if (curr <= n_seq) {
        seq_t diff = n_seq - curr;
        seq_t s = floor_sqrt(diff);
        if (s * s == diff) {
            ++count;
        }
    }

    /* Generate subsequent Fibonacci numbers (including the second 1 at F(2)). */
    while (1) {
        seq_t next = prev + curr;      /* next Fibonacci number */
        if (next > n_seq) {
            break;
        }

        seq_t diff = n_seq - next;
        seq_t s = floor_sqrt(diff);
        if (s * s == diff) {
            ++count;
        }

        prev = curr;
        curr = next;
    }

    return count;
}
