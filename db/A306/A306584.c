/*
 * A306584  For any n >= 0, let f_n be the representation of n in the
 *          factorial number system: for any i >= 0, 0 <= f_n(i) <= i and n =
 *          Sum_{i >= 0} f_n(i) * i!. The representation of a(n) in the
 *          factorial number system, say g, satisfies g(i) = f_n(f_n(i)) for
 *          any i >= 0.
 * https://oeis.org/A306584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the number obtained by applying the factorial
 *            representation of n twice: g(i)=f(f(i)) for all i≥0; a(n)=∑
 *            g(i)·i!
 * Range:     u8 n <= 255, u16 n <= 5040, u32 n <= 5040, u64 n <= 5040,
 *            u128 n <= 5040, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* factoradic digits of n: f[0] = 0, 0 ≤ f[i] ≤ i   */
    idx_t f[32] = {0};          /* 32 enough for 64‑bit n (20!) */
    idx_t temp = n;
    idx_t i = 1;
    while (temp != 0) {
        f[i] = temp % (i + 1);   /* digit for position i */
        temp /= (i + 1);
        ++i;
    }
    idx_t limit = i;            /* indices 0 .. limit-1 are used */

    /* compute a(n) = Σ f[f[i]]·i! */
    seq_t sum  = 0;
    seq_t fact = 1;             /* 0! */

    /* i = 0 term */
    sum += (seq_t)f[f[0]] * fact;

    for (idx_t j = 1; j < limit; ++j) {
        fact *= j;               /* j! */
        idx_t idx = f[j];        /* f(j) ≤ j < limit */
        seq_t gj  = (seq_t)f[idx];
        sum += gj * fact;
    }

    return sum;
}
