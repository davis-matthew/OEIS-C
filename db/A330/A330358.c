/*
 * A330358  a(n) = n mod 5 + n mod 25 + ... + n mod 5^k, where 5^k <= n <
 *          5^(k+1).
 * https://oeis.org/A330358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum_{j>=1, 5^j <= n} (n mod 5^j)
 * Range:     u8 n <= 742, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   Complexity: O(log_5(n)) */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    if (n < 5)          /* for n = 1..4 no terms; 5 contributes 0 */
        return sum;

    idx_t p = 5;        /* current power of 5 */
    /* iterate over powers of 5 that do not exceed n */
    while (p <= n) {
        sum += (seq_t)(n % p);   /* add n mod 5^j */
        /* prevent overflow when multiplying by 5 */
        if (p > n / 5)
            break;
        p *= 5;
    }
    return sum;
}
