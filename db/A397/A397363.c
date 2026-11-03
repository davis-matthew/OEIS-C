/*
 * A397363  a(n) is the trace of the product of two Hankel n X n matrices M_n
 *          and N_n whose M[i,j] and N[i,j] generic elements are respectively
 *          (i+j) in base 10 and (i+j) written in base 3 but then read as it
 *          were written in base 10.
 * https://oeis.org/A397363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum_{s=2}^{2n} s * g(s) * count_s where count_s = (s-1)
 *            for s <= n+1, else (2n - s + 1), and g(s) is s written in base
 *            3, read as a decimal integer
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 34 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* for size_t, if needed */

static seq_t base3_to_dec(idx_t x)
{
    seq_t result = 0;
    seq_t mult   = 1;
    while (x) {
        int digit = x % 3;              /* digit in base 3, 0..2 */
        result += (seq_t)digit * mult;
        mult   *= (seq_t)10;             /* next decimal place */
        x     /= 3;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n<1, but safe */

    seq_t total   = 0;
    idx_t limit   = 2 * n;
    idx_t n_plus  = n + 1;

    for (idx_t s = 2; s <= limit; ++s) {
        seq_t g      = base3_to_dec(s);
        seq_t count;
        if (s <= n_plus)
            count = (seq_t)(s - 1);
        else
            count = (seq_t)(2 * n - s + 1);

        total += g * count * (seq_t)s;
    }
    return total;
}
