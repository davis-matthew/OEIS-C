/*
 * A343605  a(n) is the greatest number < n with the same sum of balanced
 *          ternary digits as n.
 * https://oeis.org/A343605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = greatest m < n such that the sum of balanced ternary *
 *            digits of m equals the sum of balanced ternary digits of n
 * Range:     i8 n <= 120, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 8031 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sum of balanced ternary digits of a value. */
static inline seq_t balanced_sum(seq_t n)
{
    seq_t sum = 0;
    while (n != 0)
    {
        seq_t r = n % 3;
        n /= 3;
        /* Adjust remainders that fall outside {-1,0,1} */
        if (r == 2)
        {
            r = -1;
            n += 1;
        }
        else if (r == -2)
        {
            r = 1;
            n -= 1;
        }
        sum += r;
    }
    return sum;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    seq_t target = balanced_sum((seq_t)n);      /* digit-sum of n */
    seq_t k = (seq_t)n - 1;                     /* start searching from n-1 */
    while (1)
    {
        if (balanced_sum(k) == target)
            return k;                           /* found the greatest < n */
        k--;                                    /* keep going downwards */
    }
}
