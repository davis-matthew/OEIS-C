/*
 * A276336  The largest digit used when n is written in greedy A001563-base
 *          (A276326).
 * https://oeis.org/A276336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 95, u16 n <= 4320, u32 n <= 4320, u64 n <= 4320,
 *            u128 n <= 4320, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Explanation:
   a(n) is the largest digit in the greedy expansion of n in the
   number system whose base values are b_k = k * k!   (A001563).
   For n > 0 let k be the largest index with b_k ≤ n.  Let
   d = ⌊n / b_k⌋ and r = n – d·b_k.  Then a(n) = max(d, a(r)),
   with a(0) = 0.  The following implementation follows this
   recurrence iteratively.  The list of base values b_k is built
   incrementally and cached between calls. */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle the offset */
    if (n == 0) {
        return (seq_t)0;
    }

    /* static cache of base values b_k = k * k!  (k ≥ 1) */
    static seq_t *bases   = NULL;
    static size_t size    = 0;    /* number of stored bases   */
    static size_t capacity= 0;    /* allocated capacity       */
    static idx_t  next_k  = 1;    /* next k to generate      */
    static seq_t fact     = 1;    /* (next_k)!  (fact holds k! for current next_k) */

    /* Extend the cache until it contains all bases ≤ n. */
    while (1) {
        /* base for current k */
        seq_t base = (seq_t)next_k * fact;  /* k * k!  */
        if (base > (seq_t)n) {
            break;
        }
        /* grow the cache if necessary */
        if (size == capacity) {
            size_t newcap = (capacity == 0) ? 8 : capacity << 1;
            seq_t *tmp = (seq_t *)realloc(bases, newcap * sizeof(seq_t));
            if (!tmp) {
                /* allocation failure – fall back to zero */
                return (seq_t)0;
            }
            bases = tmp;
            capacity = newcap;
        }
        bases[size++] = base;
        /* prepare for the next k: increment k and update factorial */
        next_k++;
        fact = fact * (seq_t)next_k;      /* now fact == next_k! */
    }

    /* iterative computation of the largest digit */
    seq_t rem      = (seq_t)n;
    seq_t maxdigit = (seq_t)0;

    while (rem > (seq_t)0) {
        /* binary search for largest base ≤ rem */
        size_t lo = 0;
        size_t hi = size - 1;
        size_t idx = 0;
        while (lo <= hi) {
            size_t mid = (lo + hi) >> 1;
            if (bases[mid] <= rem) {
                idx = mid;
                lo  = mid + 1;
            } else {
                if (mid == 0) {
                    break;   /* avoid underflow */
                }
                hi = mid - 1;
            }
        }

        seq_t base = bases[idx];
        seq_t d    = rem / base;
        rem        = rem - d * base;
        if (d > maxdigit) {
            maxdigit = d;
        }
    }

    return maxdigit;
}
