/*
 * A375428  The maximum exponent in the unique factorization of n in terms of
 *          distinct terms of A115975 using the Zeckendorf representation of
 *          the exponents in the prime factorization of n; a(1) = 0.
 * https://oeis.org/A375428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = maximum Fibonacci number ≤ the largest prime‑exponent in
 *            the factorization of n (for n ≥ 2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 0;          /* a(1) = 0 */

    idx_t temp = n;
    idx_t max_exp = 0;
    idx_t count;

    /* factor 2 */
    if ((temp & 1) == 0) {
        count = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;
            ++count;
        }
        if (count > max_exp) max_exp = count;
    }

    /* odd factors */
    for (idx_t i = 3; i <= temp / i; i += 2) {
        if (temp % i == 0) {
            count = 0;
            while (temp % i == 0) {
                temp /= i;
                ++count;
            }
            if (count > max_exp) max_exp = count;
        }
    }

    /* remaining prime > 1 (exponent 1) */
    if (temp > 1 && (idx_t)1 > max_exp) max_exp = 1;

    /* largest Fibonacci ≤ max_exp */
    if (max_exp == 0) return 0;   /* should only occur for n == 1 */

    idx_t fib_prev = 1;
    idx_t fib_cur  = 1;
    while (fib_cur <= max_exp) {
        idx_t next = fib_prev + fib_cur;
        fib_prev = fib_cur;
        fib_cur  = next;
    }

    return (seq_t)fib_prev;
}
