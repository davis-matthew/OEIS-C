/*
 * A317946  Additive with a(p^e) = A011371(e); the 2-adic valuation of
 *          A317934(n).
 * https://oeis.org/A317946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{p^e‖n} (e – popcount(e)) (additive function where
 *            a(p^e) = exponent of 2 in e! = e – popcount(e)) Offsetting by 1:
 *            a(1) = 0
 * Range:     u8 n <= 9842, u16 n <= 9842, u32 n <= 9842, u64 n <= 9842,
 *            u128 n <= 9842, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned int popcount64(idx_t x)
{
    unsigned int c = 0;
    while (x) {
        c += (unsigned int)(x & 1);
        x >>= 1;
    }
    return c;
}

seq_t a(idx_t n)
{
    seq_t res = 0;
    idx_t m = n;

    /* Handle factor 2 (later, to skip even trial divisions) */
    if ((m & 1) == 0) {
        idx_t e = 0;
        while ((m & 1) == 0) {
            ++e;
            m >>= 1;
        }
        seq_t contrib = (seq_t)e - (seq_t)popcount64(e);
        res += contrib;
    }

    /* Trial division by odd primes up to sqrt(m) */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                ++e;
                m /= p;
            }
            seq_t contrib = (seq_t)e - (seq_t)popcount64(e);
            res += contrib;
        }
    }

    /* Remaining prime factor (if any) has exponent 1, contributes 0 */
    return res;
}
