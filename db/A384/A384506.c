/*
 * A384506  a(n) = 2^(n-7)*(n^4 - 6*n^3 + 59*n^2 - 54*n)/3.
 * https://oeis.org/A384506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-7)*(n^4 - 6*n^3 + 59*n^2 - 54*n)/3 (valid for n ≥ 7)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle small indices where the general formula would use a negative exponent. */
    if (n < 7) {
        switch (n) {
        case 0: case 1: return (seq_t)0;
        case 2: return (seq_t)1;
        case 3: return (seq_t)6;
        case 4: return (seq_t)25;
        case 5: return (seq_t)90;
        case 6: return (seq_t)300;
        default: return (seq_t)0; /* never reached */
        }
    }

    seq_t nn = (seq_t)n;

    /* Compute the 4th‑degree polynomial.  All operations are exact in seq_t. */
    seq_t poly = nn*nn*nn*nn
               - (seq_t)6 * nn*nn*nn
               + (seq_t)59 * nn*nn
               - (seq_t)54 * nn;

    /* The polynomial is always divisible by 3 for n ≥ 7. */
    seq_t val = poly / (seq_t)3;

    /* 2^(n-7) – use the provided big‑power helper. */
    seq_t pow = pow2(n - 7);

    return val * pow;
}
