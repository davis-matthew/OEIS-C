/*
 * A243360  a(n) = arrange digits of concatenation of divisors of n (A037278,
 *          A176558) in decreasing order (in base 10).
 * https://oeis.org/A243360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = arrange digits of concatenation of divisors of n
 *            (A037278, A176558) in decreasing order
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 23, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    unsigned char cnt[10] = {0};
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t div1 = d;
            do { cnt[div1 % 10]++; div1 /= 10; } while (div1);
            idx_t div2 = n / d;
            if (div2 != d) {
                do { cnt[div2 % 10]++; div2 /= 10; } while (div2);
            }
        }
    }
    seq_t res = 0;
    for (int digit = 9; digit >= 0; --digit) {
        unsigned c = cnt[digit];
        while (c--) {
            res = res * 10 + digit;
        }
    }
    return res;
}
