/*
 * A357683  a(n) = Sum_{k=0..floor(n/2)} n^k * |Stirling1(n,2*k)|.
 * https://oeis.org/A357683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} n^k * |Stirling1(n,2k)|
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* The integer types seq_t and idx_t are supplied by the build. */

seq_t a(idx_t n)
{
    /* Static state for memoisation and Stirling numbers */
    static idx_t  max_computed = 0;      /* largest n for which a(n) is known   */
    static seq_t *memo        = NULL;    /* memo[i] = a(i) for 0 <= i <= max_computed */
    static size_t memo_cap    = 0;       /* current capacity of memo            */
    static seq_t *prev        = NULL;    /* previous Stirling row (size cur+1)  */
    static size_t prev_size   = 0;       /* previous row length                 */

    /* Initialise on first call */
    if (memo == NULL) {
        memo = (seq_t*)malloc(sizeof(seq_t));
        if (memo == NULL) return (seq_t)0;   /* allocation failure */
        memo[0] = (seq_t)1;                  /* a(0) = 1              */
        memo_cap = 1;
        max_computed = 0;

        prev = (seq_t*)malloc(sizeof(seq_t));
        if (prev == NULL) return (seq_t)0;
        prev[0] = (seq_t)1;                  /* Stirling1(0,0) = 1    */
        prev_size = 1;
    }

    /* Return cached value if already computed */
    if (n <= max_computed)
        return memo[n];

    /* Grow memo array if needed */
    if ((size_t)(n + 1) > memo_cap) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(memo, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;
        memo = tmp;
        memo_cap = new_cap;
    }

    /* Compute Stirling rows incrementally up to n */
    for (idx_t cur = max_computed + 1; cur <= n; ++cur) {
        size_t cur_size = (size_t)cur + 1;
        seq_t *curr = (seq_t*)malloc(cur_size * sizeof(seq_t));
        if (curr == NULL) return (seq_t)0;   /* allocation failure */

        curr[0] = (seq_t)0;   /* Stirling1(cur,0) = 0 for cur > 0 */

        /* Recurrence: s(cur,k) = s(cur-1,k-1) + (cur-1)*s(cur-1,k) */
        for (size_t k = 1; k <= cur; ++k) {
            seq_t a_val = (k-1 < prev_size) ? prev[k-1] : (seq_t)0;
            seq_t b_val = (k   < prev_size) ? prev[k]   : (seq_t)0;
            curr[k] = a_val + (seq_t)(cur - 1) * b_val;
        }

        /* Compute a(cur) = Σ_{k=0..⌊cur/2⌋} cur^k * |Stirling1(cur,2k)| */
        seq_t power = (seq_t)1;   /* cur^0 */
        seq_t sum   = (seq_t)0;
        idx_t max_k = cur / 2;
        for (idx_t k = 0; k <= max_k; ++k) {
            if (k > 0) power = power * (seq_t)cur;   /* cur^k */
            sum += power * curr[2 * k];
        }
        memo[cur] = sum;

        /* Prepare next iteration: free old row, keep current row */
        free(prev);
        prev = curr;
        prev_size = cur_size;
    }

    max_computed = n;
    return memo[n];
}
