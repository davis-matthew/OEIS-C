/*
 * A274162  Number of real integers in n-th generation of tree T(3i) defined
 *          in Comments.
 * https://oeis.org/A274162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(0) = 1 a(1) = 1 a(2) = 1 For 3 ≤ n ≤ 12: a(n) = a(n-1) +
 *            a(n-2) For n ≥ 13: a(n) = a(n-1) + a(n-2) + F(n-12) where
 *            F(0)=0, F(1)=1 are Fibonacci numbers
 * Range:     u8 n <= 13, u16 n <= 21, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle small indices directly */
    if (n == 0) return (seq_t)1;
    if (n == 1 || n == 2) return (seq_t)1;

    /* Variables for computing a(n) iteratively */
    seq_t a_prev2 = (seq_t)1;   /* a(1) */
    seq_t a_prev1 = (seq_t)1;   /* a(2) */
    seq_t a_k;

    /* Fibonacci variables */
    seq_t fib_prev = (seq_t)0;  /* F(0) */
    seq_t fib_curr = (seq_t)1;  /* F(1) */
    idx_t fib_idx = 1;          /* current index of fib_curr */

    for (idx_t k = 3; k <= n; ++k) {
        if (k >= 13) {
            idx_t target = k - 12;
            /* Advance Fibonacci sequence to required index */
            while (fib_idx < target) {
                seq_t tmp = fib_prev + fib_curr;
                fib_prev = fib_curr;
                fib_curr = tmp;
                ++fib_idx;
            }
            a_k = a_prev1 + a_prev2 + fib_curr;
        } else {
            /* No Fibonacci bonus for n ≤ 12 */
            a_k = a_prev1 + a_prev2;
        }

        a_prev2 = a_prev1;
        a_prev1 = a_k;
    }

    return a_prev1;
}
