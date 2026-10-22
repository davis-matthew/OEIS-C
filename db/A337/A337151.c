/*
 * A337151  a(n) = (n!)^2 * Sum_{k=0..n} (-1)^(n-k) * (k+1) / ((n-k)!)^2.
 * https://oeis.org/A337151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n!)^2 * Sum_{k=0..n} (-1)^(n-k) * (k+1) / ((n-k)!)^2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          = Σ_{j=0..n} (-1)^j * (n-j+1) * (n! / j!)^2 */

/* The implementation evaluates the sum directly using only integer
   arithmetic.  The term (n! / j!)^2 is built iteratively: start with
   j = n where that value is 1, and for each step j -> j-1 multiply by
   j^2.  The alternating signs are handled by keeping separate sums
   for positive and negative terms to avoid negative intermediate
   values.  All operations use only +, -, *, and the seq_t/idx_t
   types supplied by the build. */

seq_t a(idx_t n)
{
    /* Positive and negative partial sums */
    seq_t pos_sum = 0;
    seq_t neg_sum = 0;

    /* (n! / j!)^2 for the current j; start with j = n */
    seq_t t = 1; /* since n! / n! = 1 */

    /* Iterate j from n down to 0 inclusive */
    for (idx_t j = n; ; --j) {
        /* (n - j + 1) is an idx_t; cast to seq_t for multiplication */
        seq_t term_coeff = (seq_t)(n - j + 1);
        seq_t term = term_coeff * t;  /* (n - j + 1) * (n! / j!)^2 */

        /* Accumulate into the appropriate sign sum */
        if ((j & 1) == 0) {          /* j even -> sign +1 */
            pos_sum += term;
        } else {
            neg_sum += term;
        }

        if (j == 0) {
            break; /* finished loop */
        }

        /* Update t for next iteration (j-1):
           t_next = t * j^2   because (n! / (j-1)!)^2 = (n! / j!)^2 * j^2 */
        seq_t j_seq = (seq_t)j;
        t *= j_seq * j_seq;
    }

    /* Result is positive sum minus negative sum */
    return pos_sum - neg_sum;
}
