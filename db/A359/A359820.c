/*
 * A359820  a(n) = 1 if n and n' are of different parity, otherwise 0. Here n'
 *          stands for the arithmetic derivative of n, A003415(n).
 * https://oeis.org/A359820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n and its arithmetic derivative n' are of different
 *            parity, else 0. * Computation: For n>=1, n' = Σ a_i * (n / p_i)
 *            where n = ∏ p_i^{a_i}. * Parity of n' is XOR over i of ((a_i &
 *            1) & ((n / p_i) & 1)). * a(0) = 0
 * Range:     u8 n <= 9838, u16 n <= 9838, u32 n <= 9838, u64 n <= 9838,
 *            u128 n <= 9838, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle offset 0 */
    if (n == 0)
        return (seq_t)0;

    /* Parity of n */
    idx_t parity_n = n & 1;
    idx_t parity_prime = 0;

    idx_t temp = n;

    /* Handle factor 2 separately to skip evens later */
    if ((temp & 1) == 0) {
        idx_t exp = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;
            ++exp;
        }
        if ((exp & 1) && ((n >> 1) & 1))
            parity_prime ^= 1;
    }

    /* Loop over odd divisors */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            if ((exp & 1) && ((n / p) & 1))
                parity_prime ^= 1;
        }
    }

    /* Remaining prime factor > sqrt(original n) */
    if (temp > 1) {
        if ((1 & 1) && ((n / temp) & 1))
            parity_prime ^= 1;
    }

    /* Different parity? */
    return ((parity_n ^ parity_prime) != 0) ? (seq_t)1 : (seq_t)0;
}
