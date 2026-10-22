/*
 * A366723  a(n) = Sum_{k=1..n} (-1)^(k-1) * binomial(floor(n/k)+4,5).
 * https://oeis.org/A366723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (-1)^{k-1} * binomial(floor(n/k)+4,5)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper to compute C(m,5) for m >= 5 using only sequence types. */
static seq_t binom5(seq_t m)
{
    seq_t res = 1;
    for (int i = 1; i <= 5; ++i) {
        res = res * (m - i + 1);
        res = res / i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* just in case, not part of official offset */

    long long sum = 0;               /* signed accumulator */
    for (idx_t k = 1; k <= n; ++k) {
        idx_t t = n / k;            /* floor(n / k) */
        seq_t choose = binom5((seq_t)t + 4);

        if (k & 1)                  /* odd k : add */
            sum += (long long)choose;
        else                         /* even k : subtract */
            sum -= (long long)choose;
    }
    return (seq_t)sum;               /* final result fits in unsigned seq_t */
}
