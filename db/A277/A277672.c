/*
 * A277672  Number of n-length words over a 9-ary alphabet {a_1,a_2,...,a_9}
 *          avoiding consecutive letters a_i, a_{i+1}.
 * https://oeis.org/A277672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A277672 – Number of n‑length words over a 9‑ary alphabet
   with no consecutive letters a_i, a_{i+1}.
   The sequence satisfies the recurrence obtained from the
   adjacency restriction:

   Let f[k] be the number of length‑t words ending with letter a_k
   (k = 0..8).  For t = 1, f[k] = 1 for all k.
   For t > 1:
        f[0]   = S
        f[k]   = S - f_old[k-1]   (k ≥ 1)
   where S = sum_{k=0}^8 f_old[k] is the total for length t‑1.
   The desired term a(t) = S = sum_{k} f[k].

   The following implementation uses a simple O(n) DP,
   with 9 integer counters and a running sum.  The algorithm
   works for any integer type seq_t supplied by the build
   (including 64‑bit and arbitrary‑precision types). */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the offset: a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* Arrays holding DP values for the current and next step.
       9 states: letters a_1 … a_9.  Zero‑based indices 0..8. */
    seq_t cur[9];
    seq_t nxt[9];

    /* Initialize for length 1: one word for each letter. */
    for (idx_t i = 0; i < 9; ++i)
        cur[i] = (seq_t)1;

    /* Sum for length 1 */
    seq_t sum = (seq_t)9;   /* a(1) */

    /* Iterate lengths 2 .. n */
    for (idx_t len = 2; len <= n; ++len)
    {
        /* First state has no forbidden predecessor */
        nxt[0] = sum;

        /* Remaining states: subtract the forbidden predecessor */
        for (idx_t i = 1; i < 9; ++i)
            nxt[i] = sum - cur[i - 1];

        /* Compute new sum for this length */
        seq_t newSum = nxt[0];
        for (idx_t i = 1; i < 9; ++i)
            newSum += nxt[i];

        /* Prepare for next iteration */
        sum = newSum;
        for (idx_t i = 0; i < 9; ++i)
            cur[i] = nxt[i];
    }

    /* After the loop, sum holds a(n) */
    return sum;
}
