/*
 * A362338  a(n) = n! * Sum_{k=0..floor(n/3)} (-k)^k / (k! * (n-3*k)!).
 * https://oeis.org/A362338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/3)} (-k)^k / (k! * (n-3*k)!)
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 11, i64 n <= 19, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: compute base^exp for integer exp >= 0 using binary exponentiation. */
static seq_t pow_int(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0)
    {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;                  /* running sum of terms */
    seq_t prod = 1;                 /* product n! / (n-3k)! for current k */
    seq_t fact = 1;                 /* k! for current k */

    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        /* Update prod and fact after handling k=0 */
        if (k > 0)
        {
            /* Append three factors: n-3(k-1), n-3(k-1)-1, n-3(k-1)-2 */
            idx_t start = n - 3 * (k - 1);
            prod *= (seq_t)start;
            prod *= (seq_t)(start - 1);
            prod *= (seq_t)(start - 2);

            fact *= (seq_t)k;       /* multiply by k to maintain k! */
        }

        seq_t pow_term = 1;
        if (k > 0)
            pow_term = pow_int(-(seq_t)k, k);  /* (-k)^k */

        /* prod_div = n! / (k! * (n-3k)!) */
        seq_t prod_div = prod / fact;

        /* term = prod_div * (-k)^k */
        seq_t term = prod_div * pow_term;

        sum += term;
    }

    return sum;
}
