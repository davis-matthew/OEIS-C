/*
 * A276474  a(n) = ((sqrt(2); sqrt(2))_n + (-sqrt(2); -sqrt(2))_n)/2, where
 *          (q; q)_n is the q-Pochhammer symbol.
 * https://oeis.org/A276474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = ((sqrt(2); sqrt(2))_n + (-sqrt(2); -sqrt(2))_n)/2 where
 *            (q; q)_n = Π_{k=1..n} (1 - q^k). The implementation uses the
 *            recurrence a_n = a_{n-1}*(1 - s_n^2) + 2*b_{n-1}*t_n, b_n =
 *            a_{n-1}*t_n + b_{n-1}*(1 - s_n^2) with s_n = 2^{⌊n/2⌋} if n
 *            even, else 0; t_n = 0 if n even, else -2^{⌊n/2⌋}. Thi...
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 10, i64 n <= 15, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t a_val = (seq_t)1;   /* a_0 */
    seq_t b_val = (seq_t)0;   /* b_0  */
    seq_t pow2  = (seq_t)1;   /* 2^{⌊k/2⌋} for current k */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t c, d;                 /* ´1 - sqrt2^k´ represented as c + d*sqrt2 */
        if ((k & 1ULL) == 0) {     /* k even: sqrt2^k = p = 2^{k/2}  */
            c = (seq_t)1 - pow2;
            d = (seq_t)0;
        } else {                   /* k odd:  sqrt2^k = p*sqrt2  */
            c = (seq_t)1;
            d = -(pow2);
        }

        seq_t new_a = a_val * c + (b_val * d) * 2;
        seq_t new_b = a_val * d + b_val * c;

        a_val = new_a;
        b_val = new_b;

        if (k & 1ULL)              /* when k is odd, next ⌊(k+1)/2⌋ = ⌊k/2⌋+1 */
            pow2 <<= 1;
    }

    return a_val;
}
