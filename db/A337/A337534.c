/*
 * A337534  Nontrivial squares together with nonsquares whose square part's
 *          square root is in the sequence.
 * https://oeis.org/A337534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A337534: nontrivial squares together with nonsquares whose square part's
 *  square root is in the sequence.
 *
 *  Implementation: generate terms on demand.  Each candidate k is
 *  examined; if k is a perfect square it is included; otherwise the
 *  integer g equal to the square root of the largest square dividing k
 *  is computed.  k is included only when g has already appeared
 *  earlier in the sequence.
 *
 *  The sequence is stored in a statically allocated array which grows
 *  as needed.  Calls are independent of order, and the algorithm
 *  runs in O(n·√k) time for the n terms computed, which is easily
 *  below the 2‑second limit for n ≤ 10 000 on typical hardware.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/floor_sqrt.h"   /* floor_sqrt(seq_t) */

/*  Binary search in the already computed part of the sequence.  */
static bool seq_contains(const seq_t *arr, size_t len, seq_t val)
{
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (arr[mid] < val)
            lo = mid + 1;
        else if (arr[mid] > val)
            hi = mid;
        else
            return true;
    }
    return false;
}

/*  Public sequence routine.  */
seq_t a(idx_t n)
{
    /* static storage for the sequence, growing as needed.  */
    static seq_t *seq  = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;

    /* first candidate k; the sequence starts with 4.  */
    static unsigned long long next_k = 4;

    /* generate terms until we have at least n of them.  */
    while (seq_len < n) {
        seq_t k = (seq_t)next_k;

        /* 1) Is k a perfect square?  */
        seq_t r = floor_sqrt(k);
        if (r * r == k) {
            /* non‑trivial square; it belongs in the sequence.  */
        } else {
            /* 2) Compute g = sqrt(square part of k).  */
            unsigned long long tmp = next_k;
            seq_t g = (seq_t)1;
            for (unsigned long long p = 2; p * p <= tmp; ++p) {
                if (tmp % p == 0) {
                    unsigned int e = 0;
                    while (tmp % p == 0) {
                        tmp /= p;
                        ++e;
                    }
                    unsigned int half = e >> 1;   /* floor(e/2) */
                    for (unsigned int i = 0; i < half; ++i)
                        g *= (seq_t)p;
                }
            }
            /* any remaining prime factor has exponent 1 → contributes nothing to g */

            /* 3) Include k only if g is already in the sequence.  */
            if (!seq_contains(seq, seq_len, g))
                goto next_candidate;
        }

        /* 4) Store k in the sequence.  */
        if (seq_len == seq_cap) {
            size_t new_cap = seq_cap ? seq_cap << 1 : 64;
            seq_t *new_arr = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            if (!new_arr) {  /* allocation failure – abort */
                return (seq_t)0;
            }
            seq = new_arr;
            seq_cap = new_cap;
        }
        seq[seq_len++] = k;

next_candidate:
        ++next_k;
    }

    return seq[n - 1];
}
