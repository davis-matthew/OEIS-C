/*
 * A333807  Sum of odd divisors of n that are < sqrt(n).
 * https://oeis.org/A333807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of odd divisors d of n with d < sqrt(n)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n <= 1, the sum is empty. */
    if (n <= 1)
        return 0;

    /* Compute the largest integer d such that d < sqrt(n).
       This is floor(sqrt(n-1)).  For n up to 10000 this loop
       runs at most 100 iterations. */
    idx_t limit = 0;
    while ((limit + 1) * (limit + 1) <= n - 1)
        ++limit;

    /* Sum all odd divisors d of n with 1 <= d <= limit. */
    seq_t sum = 0;
    for (idx_t d = 1; d <= limit; ++d) {
        if ((n % d) == 0 && (d & 1) != 0)
            sum += (seq_t)d;
    }
    return sum;
}
