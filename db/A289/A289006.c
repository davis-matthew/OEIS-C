/*
 * A289006  Conversion to octal of the binary expansion given by the first n
 *          terms of the period-3 sequence A011655 (repeat 0, 1, 1).
 * https://oeis.org/A289006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = d(n) where r = n mod 3 k = floor(n / 3) if r == 0 k =
 *            floor((n-1) / 3) if r == 1 k = floor((n-2) / 3) if r == 2 r == 0
 *            : result is the decimal integer consisting of k digits of '3' r
 *            == 1 : result is the decimal integer consisting of k digits of
 *            '6' r == 2 : result is the decimal integer '...
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 30, u64 n <= 59, u128 n <= 117,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 3;
    idx_t k;
    seq_t result = 0;

    if (r == 0) {                     /* n ≡ 0 (mod 3) → k = n/3 */
        k = n / 3;
        for (idx_t i = 0; i < k; ++i) {
            result = result * (seq_t)10 + (seq_t)3;
        }
    } else if (r == 1) {              /* n ≡ 1 (mod 3) → k = (n-1)/3 */
        k = (n - 1) / 3;
        for (idx_t i = 0; i < k; ++i) {
            result = result * (seq_t)10 + (seq_t)6;
        }
    } else {                          /* r == 2 → n ≡ 2 (mod 3) → k = (n-2)/3 */
        k = (n - 2) / 3;
        result = (seq_t)1;
        for (idx_t i = 0; i < k; ++i) {
            result = result * (seq_t)10 + (seq_t)5;
        }
    }

    return result;
}
