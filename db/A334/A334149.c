/*
 * A334149  a(n) is the number of terms required beyond the starting value n
 *          before a value larger than n first appears when following the same
 *          rules as Recamán's sequence A005132 but starting at n instead of
 *          0.
 * https://oeis.org/A334149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 452, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recamán‑like sequence starting at n.
   a(n) = number of terms beyond the starting value n before
          a value larger than n first appears.
   The sequence follows the standard Recamán rule:
        a(0) = n
        a(i) = a(i-1) - i   if a(i-1) - i >= 0 and not seen before
        a(i) = a(i-1) + i   otherwise
   The implementation keeps a bitset of the values that have been
   encountered (only values 0 … n need to be tracked, because we stop
   as soon as a value > n appears).
   Complexity : O(a(n)) steps and O(n) in memory. */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The current term of the sequence */
    seq_t cur = (seq_t)n;

    /* Bitset of visited values 0 … n (size n+1) */
    size_t size = (size_t)n + 1;
    bool *seen = (bool *)calloc(size, sizeof(bool));
    if (!seen) /* allocation failure – return 0 as error code */
        return (seq_t)0;

    /* Mark the starting value as seen */
    seen[(size_t)n] = true;

    idx_t step = 1;                /* number of steps beyond the start */

    for (;;) {
        seq_t cand;

        /* Recamán rule: subtract if possible and not seen yet */
        if (cur >= (seq_t)step) {                   /* cur - step >= 0 */
            seq_t temp = cur - (seq_t)step;
            /* temp is always <= n because cur <= n and we stop before
               cur ever becomes > n, so temp <= n */
            if (!seen[(size_t)temp])
                cand = temp;
            else
                cand = cur + (seq_t)step;
        } else {
            cand = cur + (seq_t)step; /* cannot subtract, must add */
        }

        /* Stop when a value strictly larger than the starting n is hit */
        if (cand > (seq_t)n) {
            free(seen);
            return (seq_t)step;
        }

        /* Record the new value and continue */
        seen[(size_t)cand] = true;
        cur = cand;
        step++;
    }
}
