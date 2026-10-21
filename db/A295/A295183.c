/*
 * A295183  a(n) = n! * [x^n] exp(n*x)/(1 - x)^n.
 * https://oeis.org/A295183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * sum_{k=0..n} binom{2n-k-1}{n-1} * n^k / k! (OEIS
 *            A295183)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a stable recurrence for the summand:
   T_{k+1} = T_k * n * (n-k) / ((k+1) * (2n-k-1)), with T_0 = n! * binom{2n-1}{n-1}. */
/* Intermediate values are held in unsigned __int128 to avoid overflow
   for the 64‑bit build when the final result still fits in seq_t.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1; /* a(0) = 1 */

    /* T = n! * binom{2n-1}{n-1} = product_{k=n}^{2n-1} k  */
    unsigned __int128 T = 1;
    for (idx_t k = n; k <= 2*n - 1; ++k)
        T *= (unsigned __int128)k;

    unsigned __int128 sum = T;

    for (idx_t j = 0; j < n; ++j) {
        /* Multiply by n and (n-j) then divide by ((j+1)*(2n-j-1)) */
        T *= (unsigned __int128)n;
        T *= (unsigned __int128)(n - j);
        unsigned __int128 den = (unsigned __int128)(j + 1) *
                                (unsigned __int128)(2*n - j - 1);
        T /= den;            /* exact division */
        sum += T;
    }

    return (seq_t)sum;
}
