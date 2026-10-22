/*
 * A355682  The "finer" of 2 representations of the Cantor middle thirds set
 *          viewed from a quarter point that lies at a(0) (the second 1 in the
 *          data).
 * https://oeis.org/A355682
 *
 * Offset:    -9    (a(-9) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = f(n) where f(0) = 1 For any integer m and -1 ≤ i ≤ 1, let
 *            x = -3·f(-m) - 9·i then f(3m+i) = x-4 if |x-4| < 3 = x+4 if
 *            |x+4| < 3 = 0 otherwise
 * Range:     i8 n <= 2187, i16 n <= 2187, i32 n <= 2187, i64 n <= 2187,
 *            i128 n <= 2187, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t f_val(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    /* Find i∈{-1,0,1} and m such that n = 3*m + i */
    idx_t i = n % 3;
    idx_t m = (n - i) / 3;
    if (i < -1) {
        i += 3;
        m -= 1;
    } else if (i > 1) {
        i -= 3;
        m += 1;
    }

    seq_t fv = f_val(-m);
    seq_t x = (- (seq_t)3) * fv - (seq_t)9 * (seq_t)i;

    seq_t d1 = x - 4;
    if (d1 < (seq_t)0)
        d1 = -d1;
    if (d1 < (seq_t)3)
        return x - 4;

    seq_t d2 = x + 4;
    if (d2 < (seq_t)0)
        d2 = -d2;
    if (d2 < (seq_t)3)
        return x + 4;

    return (seq_t)0;
}

seq_t a(idx_t n)
{
    return f_val(n);
}
