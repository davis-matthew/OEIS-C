/*
 * A257961  List of permutations of the intervals of numbers [0,F(n)) defined
 *          by x -> x*F(n-1) mod F(n), where F(n) is the n-th Fibonacci number
 *          A000045.
 * https://oeis.org/A257961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = ((n - S) * F(m-1)) mod F(m), where S = sum_{i=2}^{m-1}
 *            F(i) and m is the smallest index with S + F(m) > n
 * Range:     u8 n <= 442, u16 n <= 9979, u32 n <= 9979, u64 n <= 9979,
 *            u128 n <= 9979, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cumulative length of all previous blocks (F(2), F(3), ...) */
    idx_t cum = 0;

    /* Fibonacci numbers: F(0)=0, F(1)=1 */
    seq_t f_prev2 = 0;   /* F(i-2) */
    seq_t f_prev1 = 1;   /* F(i-1) */

    for (idx_t i = 2; ; ++i)
    {
        seq_t Fi = f_prev1 + f_prev2;   /* F(i)   */
        seq_t mul = f_prev1;            /* F(i-1) */

        /* how many positions remain to reach index n? */
        idx_t remaining = n - cum;

        /* if the target is inside this block */
        if (Fi > (seq_t)remaining)
        {
            idx_t offset = remaining;              /* k */
            seq_t result = ((seq_t)offset * mul) % Fi;
            return result;
        }

        /* otherwise the target lies after this block */
        /* Fi fits in idx_t because Fi <= remaining <= n */
        cum += (idx_t)Fi;

        /* advance Fibonacci numbers */
        f_prev2 = f_prev1;
        f_prev1 = Fi;
    }
}
