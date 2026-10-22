/*
 * A300488  a(n) = n! * [x^n] -exp(n*x)*log(1 - x)/(1 - x).
 * https://oeis.org/A300488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * [x^n] -exp(n*x)*log(1 - x)/(1 - x)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a direct summation with O(n) time and O(1) extra memory. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Forward pass to compute n! and s_n = n! * H(n) */
    seq_t fact = 1;      /* k!   */
    seq_t s    = 0;      /* k!·H(k) */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t f_prev = fact;          /* (k-1)!  */
        fact = fact * (seq_t)k;       /* k!   */
        s    = (seq_t)k * s + f_prev; /* k·s_{k-1} + f_{k-1} = s_k */
    }

    /* Descend from k = n to 1 computing the sum */
    seq_t sum   = 0;
    seq_t pow   = 1;      /* n^0  for k = n */
    seq_t binom = 1;      /* C(n,n) */
    idx_t k = n;

    for (;;) {
        /* term = n^{n-k} * C(n,k) * (k!·H(k)) */
        seq_t term = pow;
        term = term * binom;
        term = term * s;
        sum = sum + term;

        if (k == 1) break;  /* finished */

        /* Update for next k-1 */
        pow = pow * (seq_t)n;                           /* n^{n-(k-1)} = n^{n-k}·n */
        seq_t tmp = binom * (seq_t)k;                   /* C(n,k) * k   */
        tmp = tmp / ((seq_t)(n - k + 1));               /* divide by n-k+1 */
        binom = tmp;

        /* Update factorial and s for k-1 using backward recurrence */
        seq_t f_prev = fact / (seq_t)k;                 /* f_{k-1} = f_k / k */
        seq_t s_prev = (s - f_prev) / (seq_t)k;         /* s_{k-1} = (s_k - f_{k-1}) / k */
        fact = f_prev;
        s    = s_prev;
        --k;
    }

    return sum;
}
