/*
 * A360168  a(n) = Sum_{k=0..floor(n/3)} binomial(2*n,n-3*k).
 * https://oeis.org/A360168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(2*n, n-3*k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Using a direct O(n) computation:
        iterate over r = 0..n computing C(2n, r) by the recurrence
        C(2n, r+1) = C(2n, r) * (2n - r) / (r + 1).
        Add the terms where r ≡ n (mod 3). */
    seq_t sum = 0;
    seq_t binom = 1;                /* C(2n, 0) */
    idx_t  r;
    idx_t  rem = n % 3;             /* target remainder */

    for (r = 0; r <= n; ++r) {
        if ((r % 3) == rem)
            sum += binom;

        if (r == n)                /* already reached the last needed term */
            break;

        /* Compute next binomial coefficient C(2n, r+1) */
        seq_t num = binom * (seq_t)(2 * n - r); /* numerator */
        binom = num / (r + 1);                  /* exact division */
    }

    return sum;
}
