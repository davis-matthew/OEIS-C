/*
 * A353828  The positions of nonzero digits in the balanced ternary expansions
 *          of n and a(n) are the same, and the k-th leftmost nonzero digit in
 *          a(n) equals the product of the k leftmost nonzero digits in n.
 * https://oeis.org/A353828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 6561, u32 n <= 6561, u64 n <= 6561,
 *            u128 n <= 6561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A353828.
   For a given n write its balanced ternary representation.
   Let d[0]..d[len-1] be the digits from most significant to least.
   For each non‑zero digit prefix product P_k = Π_{i=0..k} d[i]
   (here d[i] ∈ {−1,1}), the corresponding digit of a(n) is P_k.
   Thus a(n) has the same non‑zero digit positions as n, and the
   k‑th leftmost non‑zero digit of a(n) equals the cumulative
   product of the first k non‑zero digits of n. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Maximum number of balanced ternary digits for uint64_t is 41.
       Use a buffer large enough for any seq_t build. */
    int8_t digits[64];
    int len = 0;

    /* Balanced ternary conversion (least significant digit first). */
    idx_t m = n;
    while (m > 0) {
        unsigned r = m % 3;
        m /= 3;
        int d;
        if (r == 0) {
            d = 0;
        } else if (r == 1) {
            d = 1;
        } else {            /* r == 2 → digit -1, carry 1 */
            d = -1;
            ++m;
        }
        digits[len++] = (int8_t)d;
    }
    if (len == 0) return (seq_t)0;  /* n == 0 */

    /* Pre‑compute powers of 3 up to exponent len-1. */
    seq_t pow[64];
    pow[0] = (seq_t)1;
    for (int k = 1; k < len; ++k) {
        pow[k] = pow[k - 1] * (seq_t)3;
    }

    seq_t result = (seq_t)0;
    int sign = 1;                           /* cumulative product of non‑zero digits seen so far */

    /* Iterate from most significant to least, accumulating a(n). */
    for (int j = len - 1; j >= 0; --j) {
        int d = digits[j];
        if (d != 0) {
            sign *= d;                     /* update cumulative product */
            if (sign > 0) {
                result += pow[j];
            } else {
                result -= pow[j];
            }
        }
    }
    return result;
}
