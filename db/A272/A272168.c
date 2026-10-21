/*
 * A272168  a(n) = Product_{k=0..n} (k^2-k)!.
 * https://oeis.org/A272168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product_{k=0..n} (k^2-k)!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_mul.h"

/* Compute a(n) = ∏_{k=0}^{n} (k^2-k)! */
seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;   /* cumulative product of factorials   */
    seq_t fact   = (seq_t)1;   /* factorial of current m = k^2-k      */
    idx_t  prev_m = 0;          /* previous value of k^2-k            */

    for (idx_t k = 0; k <= n; ++k)
    {
        idx_t m = k * k - k;    /* k^2 - k (fits in idx_t for n up to ~2^32) */

        /* Extend the factorial from prev_m to m by successive
           multiplications.  The loop runs only when m increases,
           so the total number of multiplications is ∑(m_k - m_{k-1})
           = O(n^2). */
        if (m > prev_m)
        {
            for (idx_t x = prev_m + 1; x <= m; ++x)
            {
                fact = seq_mul(fact, (seq_t)x);
            }
            prev_m = m;
        }

        /* Multiply the running product by the new factorial value. */
        result = seq_mul(result, fact);
    }

    return result;
}
