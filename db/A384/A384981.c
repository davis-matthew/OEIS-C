/*
 * A384981  Number of proper vertex colorings of the n-complete bipartite
 *          graph using exactly 5 interchangeable colors.
 * https://oeis.org/A384981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^(n-1) - (5/3)*2^(2*n-2) - 2*3^(n-1) + 2^(n+1) - 4/3
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Small power helper using binary exponentiation.  Works for any seq_t. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (exp > 0)
            base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;        /* undefined for n < 1, but guard for safety */

    /* Term 1: 3 * 6^(n-1) */
    seq_t term1 = pow_seq((seq_t)6, n - 1) * (seq_t)3;

    /* Term 2: 5 * 2^(2*n-2) */
    idx_t exp2 = 2 * n - 2;
    seq_t term2 = pow_seq((seq_t)2, exp2) * (seq_t)5;

    /* Term 3: 6 * 3^(n-1) */
    seq_t term3 = pow_seq((seq_t)3, n - 1) * (seq_t)6;

    /* Term 4: 3 * 2^(n+1) */
    seq_t term4 = pow_seq((seq_t)2, n + 1) * (seq_t)3;

    /* Compute 3*a(n) = term1 - term2 - term3 + term4 - 4 */
    seq_t result = term1 - term2 - term3 + term4 - (seq_t)4;

    /* Exact division by 3 */
    result /= (seq_t)3;

    return result;
}
