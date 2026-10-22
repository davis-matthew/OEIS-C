/*
 * A324177  Integers k such that floor(sqrt(k)) + floor(sqrt(k/4)) divides k.
 * https://oeis.org/A324177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Return the nth integer k such that floor(sqrt(k)) +
 *            floor(sqrt(k/4)) divides k
 * Range:     u8 n <= 24, u16 n <= 384, u32 n <= 3200, u64 n <= 3200,
 *            u128 n <= 3200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Integer square root of a 64‑bit value, implemented by binary search. */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = 4294967296ULL;          /* 2^32  (> sqrt(2^64-1)) */
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        if ((__uint128_t)mid * mid <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;                             /* floor(sqrt(x)) */
}

/* Return the nth term of sequence A324177. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Persistent data across calls */
    static seq_t *terms = NULL;
    static size_t size  = 0;       /* number of terms stored */
    static size_t cap   = 0;       /* allocated capacity */

    /* Current state of the scan (next k to test) */
    static idx_t  last_k    = 1;   /* next integer to examine   */
    static idx_t  s         = 1;   /* floor(sqrt(last_k))      */
    static idx_t  next_s_sq = 4;   /* (s+1)^2  to trigger an s increment */
    static idx_t  q         = 0;   /* last_k / 4                */
    static idx_t  t         = 0;   /* floor(sqrt(q))            */

    while (size < n) {
        seq_t k = (seq_t)last_k;
        idx_t d = s + t;
        if (d != 0 && (last_k % d) == 0) {
            /* Grow storage if needed */
            if (size >= cap) {
                size_t newcap = (cap == 0) ? 16 : (cap << 1);
                seq_t *tmp = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
                if (!tmp) abort();      /* allocation failed */
                terms = tmp;
                cap   = newcap;
            }
            terms[size++] = k;
        }

        /* Advance to next integer k */
        last_k++;

        /* Update s when k reaches the next square */
        if (last_k == next_s_sq) {
            s++;
            next_s_sq = (s + 1) * (s + 1);
        }

        /* Update q = floor(k/4) and t when q changes */
        idx_t new_q = last_k >> 2;
        if (new_q != q) {
            q = new_q;
            t = isqrt_u64(q);
        }
    }

    return terms[n - 1];   /* 0‑based indexing of the terms array */
}
