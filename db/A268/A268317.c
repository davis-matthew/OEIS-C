/*
 * A268317  Irregular triangle read by rows: T(n,k) gives the columns sum in
 *          the table Fib(n+1) X Fib(n), where k = 1..Fib(n), and 1's are
 *          assigned to cells on the longest diagonal path.
 * https://oeis.org/A268317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   a(n) is the column‐sum of the longest diagonal path in the *
 *            Fib(n+1)×Fib(n) table (row‐wise read). For n≥1 the value is *
 *            (down steps in that column) + 1 where the down steps are *
 *            floor(k*Fib(n+1)/Fib(n)) - floor((k-1)*Fib(n+1)/Fib(n)) for * k
 *            < Fib(n). For the last column k = Fib(n) it is * Fi...
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Output exactly one fenced C code block. */
seq_t a(idx_t n)
{
    /* a(0) is the offset term. */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* Find the triangle row r (1‑based) that contains index n.  */
    /* Cumulative sum of Fib(1..r)  = Fib(r+2)-1.  We generate
     * Fibonacci numbers until Fib(r+2)>n+1.  */
    idx_t prev_sum = (idx_t)0;   /* sum of Fib(1..r-1) */
    idx_t fib_prev = (idx_t)0;   /* Fib(r-1) */
    idx_t fib_curr = (idx_t)1;   /* Fib(r)   */
    idx_t r = (idx_t)0;          /* row counter */

    while (1) {
        ++r;                    /* r now refers to Fib(r) */
        idx_t fib_r = fib_curr; /* Fib(r)       */
        if (n <= prev_sum + fib_r)   /* r is the desired row */
            break;
        prev_sum += fib_r;      /* update cumulative sum */

        /* advance Fibonacci sequence: Fib(r+1) = fib_prev + fib_curr */
        idx_t fib_next = fib_prev + fib_curr;
        fib_prev = fib_curr;
        fib_curr = fib_next;
    }

    /* At this point: */
    /*   fib_curr   = Fib(r)        = C (columns in this row)   */
    /*   fib_prev   = Fib(r-1)      (previous Fibonacci)      */
    /*   Fib(r+1) = fib_prev + fib_curr = R (rows in this table)  */
    idx_t C   = fib_curr;
    idx_t R   = fib_prev + fib_curr;    /* Fib(r+1)          */

    /* Column index within the row (1‑based). */
    idx_t k = n - prev_sum;             /* 1 ≤ k ≤ C          */

    /* Compute the number of down steps in column k. */
    idx_t d;        /* down steps (0,1, or 2) */

    if (k < C) {
        /* For k < C, use floor(k*R/C) - floor((k-1)*R/C). */
        idx_t num  = (k * R) / C;
        idx_t num1 = ((k - 1) * R) / C;
        d = num - num1;
    } else { /* k == C, last column */
        /* sum of down steps in first (C-1) columns = floor((C-1)*R / C) */
        idx_t sum_prev = ((C - 1) * R) / C;
        /* total down steps in the row = R-1 */
        d = (R - 1) - sum_prev;
    }

    /* The column sum is down steps + 1. */
    return (seq_t)(d + 1);
}
