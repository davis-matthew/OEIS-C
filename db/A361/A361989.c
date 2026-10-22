/*
 * A361989  a(n) is the sum of the Fibonacci numbers missing from the dual
 *          Zeckendorf representation of n; a(0) = 0, and for n > 0, a(n) =
 *          A022290(A035327(A003754(n+1))).
 * https://oeis.org/A361989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   a(n) = F(k+1)-1 - (n - (F(k+2)-1)) where k is the largest
 *            integer such that F(k+2)-1 ≤ n. This is equivalent to the
 *            definition "sum of the Fibonacci numbers missing from the dual
 *            Zeckendorf representation of n"
 * Range:     u8 n <= 87, u16 n <= 9052, u32 n <= 9052, u64 n <= 9052,
 *            u128 n <= 9052, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /*  a(0) = 0 and a(1) = 0 are handled directly. */
    if (n < 2)
        return 0;

    /*  Generate Fibonacci numbers iteratively until the cumulative
        sum exceeds `n`.  `f_prev` holds F(i-1), `f_curr` holds F(i),
        and `sumS` holds the sum S_i = F(1)+...+F(i).  */
    seq_t f_prev = 1;          /* F(1)   */
    seq_t f_curr = 1;          /* F(2)   */
    seq_t sumS   = f_prev + f_curr;  /* S_2 = F(1)+F(2) = 2 */
    seq_t last_fib = f_curr;   /* value of the last added Fibonacci number */
    idx_t  i = 2;              /* current index i for which sumS = S_i */

    while (sumS <= n)
    {
        seq_t f_next = f_prev + f_curr;   /* F(i+1)  */
        f_prev = f_curr;                  /* shift */
        f_curr = f_next;
        sumS += f_next;                   /* add F(i+1) to the cumulative sum */
        last_fib = f_next;
        i++;                              /* now i corresponds to the new index */
    }

    /*  Backtrack one step: the last addition caused sumS to exceed n.
        After subtraction we have the largest S_k ≤ n.  */
    sumS -= last_fib;
    i--;          /* not really needed, but kept for clarity */

    /*  Compute the missing‑Fibonacci sum.  */
    seq_t diff = (seq_t)n - sumS;         /* n - S_k  (0 ≤ diff ≤ F(k+1)-1) */
    seq_t result = f_curr - 1 - diff;     /* F(k+1)-1 - diff  */

    return result;
}
