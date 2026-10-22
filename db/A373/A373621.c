/*
 * A373621  Expansion of 1 / ( (1 - 25*x^5) * (1 - x/(1 - 25*x^5)^(1/5)) ).
 * https://oeis.org/A373621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/5⌋} 25^k * binomial(n/5, k) * where
 *            binomial(x,k) = x*(x-1)*…*(x-k+1)/k! . * The terms are computed
 *            via the recurrence * T_0 = 1, * T_k = T_{k-1} * (n - 5*(k-1)) *
 *            5 / k (k ≥ 1) * and a(n) = Σ_{k=0..⌊n/5⌋} T_k
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 32, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for the sequence A373621 */
seq_t a(idx_t n)
{
    idx_t k_max = n / 5;          /* maximum k in the sum             */
    seq_t sum  = 1;                /* T_0 contributes 1               */
    seq_t term = 1;                /* T_k will hold current term      */
    for (idx_t k = 1; k <= k_max; ++k) {
        term = term * (seq_t)(n - 5 * (k - 1)) * 5;
        term = term / k;           /* exact division, no remainder    */
        sum += term;               /* accumulate the term             */
    }
    return sum;
}
