/*
 * A330348  a(n) is the number of divisors of n whose last digit equals the
 *          last digit of n.
 * https://oeis.org/A330348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = #{d | d divides n and d % 10 == n % 10}
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of divisors of n that end with the same digit as n. */

seq_t a(idx_t n)
{
    /* Zero is not part of the defined range, but returning 0 avoids UB */
    if (n == 0) return 0;

    seq_t cnt = 0;
    unsigned last_digit = (unsigned)(n % 10);

    /* Count divisors by iterating up to the integer square root of n.
       Use the comparison i <= n/i to avoid overflow. */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            if ((i % 10) == last_digit) ++cnt;
            idx_t j = n / i;
            if (j != i && (j % 10) == last_digit) ++cnt;
        }
    }
    return cnt;
}
