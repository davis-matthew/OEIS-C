/*
 * A269533  Number of length-n 0..4 arrays with no repeated value differing
 *          from the previous repeated value by other than one.
 * https://oeis.org/A269533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*a(n-1) - 65*a(n-2) + 80*a(n-3) + 163*a(n-4) -
 *            280*a(n-5) - 208*a(n-6); (a(1)=5, a(2)=25, a(3)=120, a(4)=568,
 *            a(5)=2648, a(6)=12214)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* memoization storage */
    static seq_t *memo = NULL;
    static idx_t memo_end = 0;        /* highest index that has been computed */
    static size_t capacity = 16;      /* current allocation size (in elements) */

    if (n < 1)
        return (seq_t)0;              /* sequence is defined only for n ≥ 1 */

    /* First call – allocate and initialize base values */
    if (!memo) {
        memo = (seq_t*)malloc(capacity * sizeof(seq_t));
        if (!memo) return (seq_t)0;   /* allocation failure */
        memo[0] = (seq_t)0;           /* unused – keep index 1 aligned */
        memo[1] = (seq_t)5;
        memo[2] = (seq_t)25;
        memo[3] = (seq_t)120;
        memo[4] = (seq_t)568;
        memo[5] = (seq_t)2648;
        memo[6] = (seq_t)12214;
        memo_end = 6;
    }

    /* If the requested index is already computed, return it */
    if (n <= memo_end)
        return memo[n];

    /* Grow the storage if necessary */
    if ((size_t)n >= capacity) {
        while ((size_t)n >= capacity)
            capacity <<= 1;
        seq_t *tmp = (seq_t*)realloc(memo, capacity * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure */
        memo = tmp;
    }

    /* Compute all missing terms up to n using the recurrence */
    for (idx_t i = memo_end + 1; i <= n; ++i) {
        seq_t val = 14 * memo[i-1]
                    - 65 * memo[i-2]
                    + 80 * memo[i-3]
                    + 163 * memo[i-4]
                    - 280 * memo[i-5]
                    - 208 * memo[i-6];
        memo[i] = val;
    }
    memo_end = n;

    return memo[n];
}
