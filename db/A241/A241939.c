/*
 * A241939  Number of length 3+4 0..n arrays with no consecutive five elements
 *          summing to more than 2*n.
 * https://oeis.org/A241939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 28*a(n-2) + 56*a(n-3) - 70*a(n-4) + 56*a(n-5)
 *            - 28*a(n-6) + 8*a(n-7) - a(n-8) for n>8 Initial values (offset
 *            1): a(1)=43, a(2)=509, a(3)=3150, a(4)=13339, a(5)=44063,
 *            a(6)=122162, a(7)=297324, a(8)=654345
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 31, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *memo = NULL;   /* memo[i] stores a(i), memo[0] unused */
    static idx_t memo_sz = 0;    /* highest index currently filled */

/* initialise first 8 terms on first call */
    if (memo == NULL) {
        memo_sz = 8;
        memo = (seq_t*)malloc((memo_sz + 1) * sizeof(seq_t));
        if (memo == NULL) return 0;        /* allocation failure */

        seq_t mask = ~(seq_t)0;            /* all bits 1 for the type */

        memo[0] = 0;
        memo[1] = 43;
        memo[2] = (seq_t)(509   & mask);
        memo[3] = (seq_t)(3150  & mask);
        memo[4] = (seq_t)(13339 & mask);
        memo[5] = (seq_t)(44063 & mask);
        memo[6] = (seq_t)(122162 & mask);
        memo[7] = (seq_t)(297324 & mask);
        memo[8] = (seq_t)(654345 & mask);
    }

/* extend cache if necessary */
    if (n > memo_sz) {
        seq_t *new_memo = (seq_t*)realloc(memo, (n + 1) * sizeof(seq_t));
        if (new_memo == NULL) return 0;   /* allocation failure */
        memo = new_memo;

        for (idx_t i = memo_sz + 1; i <= n; ++i) {
            seq_t val = 8 * memo[i-1];
            val -= 28 * memo[i-2];
            val += 56 * memo[i-3];
            val -= 70 * memo[i-4];
            val += 56 * memo[i-5];
            val -= 28 * memo[i-6];
            val += 8  * memo[i-7];
            val -= memo[i-8];
            memo[i] = val;
        }
        memo_sz = n;
    }

    return memo[n];
}
