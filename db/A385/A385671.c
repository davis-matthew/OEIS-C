/*
 * A385671  a(n) = Sum_{k=0..n} 2^k * 3^(n-k) * binomial(4*n+1,k) *
 *          binomial(4*n-k,n-k).
 * https://oeis.org/A385671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 2^k * 3^(n-k) * binomial(4*n+1,k) *
 *            binomial(4*n-k,n-k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We compute the value using the equivalent, simpler expression  
   a(n) = Sum_{k=0..n} binomial(4*n+1,k) * 5^k * (-3)^(n-k). */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Initial power of 3^n (for the magnitude of (-3)^{n-k}) */
    seq_t pow3 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow3 *= 3;

    seq_t pow5 = 1;          /* 5^0 */
    seq_t sum_even = 0;     /* sum of terms with even parity of (n-k) */
    seq_t sum_odd  = 0;     /* sum of terms with odd  parity of (n-k) */

    seq_t binom = 1;         /* C(4n+1,0) */
    seq_t m = (seq_t)n * 4 + 1;   /* m = 4n+1 */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* term = binom * 5^k * 3^(n-k)  (all positive integers) */
        seq_t term = binom;
        term *= pow5;
        term *= pow3;

        /* Add term to the appropriate sum according to parity of (n-k) */
        if ( ((n - k) & 1) == 0 )
            sum_even += term;
        else
            sum_odd += term;

        /* Prepare values for next iteration (k < n) */
        if (k < n)
        {
            /* binom_{k+1} = binom_k * (m - k) / (k + 1) */
            binom = binom * (m - (seq_t)k) / ((seq_t)(k + 1));
            pow5 *= 5;
            pow3 /= 3;      /* exact division by 3 */
        }
    }

    /* Final result: difference of the two sums (always non‑negative) */
    seq_t result = sum_even;
    result -= sum_odd;
    return result;
}
