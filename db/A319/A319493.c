/*
 * A319493  a(n) = 1*2 - 3 + 4*5 - 6 + 7*8 - 9 + 10*11 - 12 + 13*14 - ... +
 *          (up to n).
 * https://oeis.org/A319493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(1) = 1 a(n) = a(n-1) + f(n) for n >= 2 where f(n) = -n if n ≡
 *            0 (mod 3) f(n) = +n if n ≡ 1 (mod 3) f(n) = (n-1)² if n ≡ 2 (mod
 *            3)
 * Range:     i8 n <= 10, i16 n <= 67, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;
    seq_t res = (seq_t)1;           /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        idx_t r = i % 3;
        if (r == 0)
            res -= (seq_t)i;
        else if (r == 1)
            res += (seq_t)i;
        else {    /* r == 2 */
            seq_t m = (seq_t)(i - 1);
            res += m * m;
        }
    }
    return res;
}
