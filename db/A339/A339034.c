/*
 * A339034  Row sums of A339033.
 * https://oeis.org/A339034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! - (n - 1)! + Sum_{k=1..n} (n + 1 - k)*(k - 1)! for n >
 *            0, with a(0)=1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the offset explicitly */
    if (n == 0) {
        return (seq_t)1;
    }

    /* fact holds (k-1)! after each iteration */
    seq_t fact = (seq_t)1;          /* 0! */
    seq_t sum  = (seq_t)0;

    /* Will store (n-1)! after the loop */
    seq_t fact_n_minus_one = (seq_t)1;

    /* Compute the sum and factorials incrementally */
    for (idx_t k = (idx_t)1; k <= n; ++k) {
        seq_t termmult = (seq_t)(n + (idx_t)1 - k); /* (n+1-k) */
        sum += termmult * fact;                     /* add term to sum */

        fact_n_minus_one = fact; /* fact is (k-1)! here */
        fact *= k;               /* advance to k! for next iteration */
    }

    /* After loop: fact = n! and fact_n_minus_one = (n-1)! */
    return fact - fact_n_minus_one + sum;
}
