/*
 * A331513  a(n) = Sum_{k=0..n} (-n)^(n-k) * (n+k+1) * binomial(n,k) *
 *          binomial(n+k,k).
 * https://oeis.org/A331513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-n+1)^k * (k+1) * binomial(n+1,k+1)^2
 *            (equivalent to a(n) = Sum_{k=0..n} (-n)^(n-k) * (n+k+1) *
 *            binomial(n,k) * binomial(n+k,k) )
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 9, i64 n <= 16, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A331513.
   External linkage: seq_t a(idx_t n)
   Uses only integer arithmetic of type seq_t and idx_t.
   Works for arbitrary‑precision seq_t and 64‑bit seq_t,
   computing in O(n) time. */
seq_t a(idx_t n)
{
    /* Conversion of the index to the arithmetic type. */
    seq_t sum   = 0;
    seq_t base  = -(seq_t)n + 1;          /* (-n + 1) */
    seq_t pow_k = 1;                      /* base^k, starts with k = 0 */
    seq_t m     = (seq_t)n + 1;           /* m = n + 1, used in binomials */

    seq_t C_prev = 1;                     /* C(m,0) */

    for (idx_t k = 0; k <= n; ++k) {
        /* r = k + 1 → we need C(m, r) */
        seq_t r   = (seq_t)(k + 1);
        seq_t num = C_prev * (m - (r - 1));
        seq_t C_r = num / r;              /* C(m, r) */

        seq_t C_sq = C_r * C_r;           /* square of the binomial */
        seq_t term = pow_k * (seq_t)(k + 1) * C_sq;  /* term contribution */

        sum += term;                      /* accumulate result */

        pow_k *= base;                    /* update base^k for next iteration */
        C_prev = C_r;                     /* prepare C(m, r) for next r */
    }

    return sum;
}
