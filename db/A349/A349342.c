/*
 * A349342  Sum of A026741 and its Dirichlet inverse.
 * https://oeis.org/A349342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A026741(n) + A349341(n), where A026741(n) = n (n odd),
 *            n/2 (n even); A349341 is multiplicative with a(2^e) = -1, a(p) =
 *            -p, a(p^e) = 0 (p odd prime, e > 1)
 * Range:     u8 n <= 128, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 102 terms through n = 8866 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t A = (n & 1) ? (seq_t)n : (seq_t)(n >> 1);

    idx_t t = n;
    seq_t mag = 1;
    int neg = 0;
    int zero = 0;

    if ((t & 1) == 0) {
        neg ^= 1;
        while ((t & 1) == 0)
            t >>= 1;
    }

    for (idx_t p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            int e = 0;
            while (t % p == 0) {
                t /= p;
                e++;
            }
            if (e == 1) {
                mag *= (seq_t)p;
                neg ^= 1;
            } else {
                zero = 1;
                break;
            }
        }
    }

    if (!zero && t > 1) {
        mag *= (seq_t)t;
        neg ^= 1;
    }

    if (zero)
        return A;
    if (neg)
        return A - mag;
    return A + mag;
}
