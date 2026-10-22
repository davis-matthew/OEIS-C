/*
 * A326399  Expansion of Sum_{k>=1} k * x^k / (1 - x^(3*k)).
 * https://oeis.org/A326399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, n/d ≡ 1 (mod 3)} d
 * Range:     u8 n <= 195, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* enumerate divisor pairs (i, j) with i * j == n */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t j = n / i;
            if ((j % 3) == 1)
                sum += (seq_t)i;
            if (i != j && (i % 3) == 1)
                sum += (seq_t)j;
        }
    }
    return sum;
}
