/*
 * A373509  Expansion of 1/(1 - x/(1 - 8*x^4)^(1/4)).
 * https://oeis.org/A373509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/4⌋} 8^k * binomial(n/4-1, k) (binomial for
 *            fractional first argument)
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The formula is implemented iteratively using factorial‑division to keep all operations on integers. */

static inline seq_t mul_div(seq_t a, seq_t b, seq_t d) {
    /* Compute (a * b) / d exactly, avoiding overflow by dividing 'a' before multiplying. */
    seq_t q = a / d;
    seq_t r = a % d;
    return q * b + (r * b) / d;
}

seq_t a(idx_t n) {
    idx_t m = n / 4;               /* number of terms in the sum (floor division) */
    seq_t term = 1;                /* term_k = contribution for current k, term_0 = 1 */
    seq_t sum  = 1;                /* sum of terms; starts with term_0 */

    for (idx_t k = 0; k < m; ++k) {
        /* Compute the next term using the recurrence:
           term_{k+1} = term_k * (2 * (n - 4*(k+1))) / (k+1)  */
        seq_t num = 2 * (seq_t)(n - 4 * (k + 1));
        seq_t denom = k + 1;
        term = mul_div(term, num, denom);
        sum += term;
    }

    return sum;
}
