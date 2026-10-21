/*
 * A244058  n-th term of the 'Reverse and Add!' sequence starting with n.
 * https://oeis.org/A244058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = g⁽ⁿ⁻¹⁾(n) where g(x) = x + reverse_decimal(x)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 20, u64 n <= 41, u128 n <= 83,
 *            big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Returns the n‑th term of A244058.  The result is obtained by
 * applying the “reverse and add” operation (x → x + reverse_decimal(x))
 * exactly n‑1 times, starting from the initial value n.  */
#include "../utils/reverse_decimal.h"

seq_t a(idx_t n)
{
    seq_t value = (seq_t)n;          /* cast the index to seq_t */
    for (idx_t i = 1; i < n; ++i) {  /* perform n‑1 reverse‑and‑add steps */
        seq_t rev = reverse_decimal(value);
        value += rev;
    }
    return value;
}
