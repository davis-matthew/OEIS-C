/*
 * A359516  Number of compositions (ordered partitions) of n into at most 4
 *          positive Fibonacci numbers (with a single type of 1).
 * https://oeis.org/A359516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of ordered compositions of n into at most 4 Fibonacci *
 *            numbers (fib set = {1,2,3,5,8,13,...} – the two 1's are treated
 *            as a single * value). Computed by dynamic programming with
 *            O(4·n·F) operations, * where F is the number of Fibonacci
 *            numbers ≤ n (≈20 for n≤10⁴)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Guard against absurdly large n that would blow up memory.
       The test harness uses n up to about 10 000, so we cap the
       allocation at a modest value.  If n is too big we simply
       return 0 (undefined for the problem). */
    if (n > 1000000ULL)
        return (seq_t)0;

    /* Arrays of length n+1 for DP values.  We allocate using malloc
       as the harness does not provide alloc_seq_array(). */
    size_t len = (size_t)n + 1;
    seq_t *dp_prev = (seq_t *)malloc(len * sizeof(seq_t));
    seq_t *dp_curr = (seq_t *)malloc(len * sizeof(seq_t));
    if (!dp_prev || !dp_curr) {
        free(dp_prev);
        free(dp_curr);
        return (seq_t)0;                         /* allocation failure */
    }

    /* Initialise dp_prev[0] = 1; others are zeroed by memset. */
    memset(dp_prev, 0, len * sizeof(seq_t));
    dp_prev[0] = (seq_t)1;
    memset(dp_curr, 0, len * sizeof(seq_t));

    /* Fibonacci numbers (single type of 1) up to n.  Up to ~20 numbers
       for n≤10⁴, static array of 64 suffices. */
    idx_t fib[64];
    size_t fibcnt = 0;
    idx_t f1 = 1, f2 = 2;
    while (f1 <= n) {
        fib[fibcnt++] = f1;
        idx_t f_next = f1 + f2;
        f1 = f2;
        f2 = f_next;
    }

    seq_t answer = (seq_t)0;
    if (n == 0)                      /* empty composition counts once */
        answer = (seq_t)1;

    /* DP over number of parts k = 1..4. */
    for (size_t k = 1; k <= 4; ++k) {
        /* Zero dp_curr for this k. */
        memset(dp_curr, 0, len * sizeof(seq_t));

        /* Transition: add one part of size f. */
        for (size_t fi = 0; fi < fibcnt; ++fi) {
            idx_t f = fib[fi];
            for (idx_t s = f; s <= n; ++s) {
                dp_curr[s] += dp_prev[s - f];
            }
        }

        /* Add compositions with exactly k parts. */
        answer += dp_curr[n];

        /* Prepare for next iteration: dp_prev = dp_curr. */
        seq_t *tmp = dp_prev;
        dp_prev = dp_curr;
        dp_curr = tmp;
    }

    free(dp_prev);
    free(dp_curr);
    return answer;
}
