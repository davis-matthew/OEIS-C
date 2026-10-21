/*
 * A252505  Number of biquadratefree (4th power free) divisors of n.
 * https://oeis.org/A252505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} (min(e,3)+1) (OEIS A252505)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1)=1, a(2)=2, a(4)=3, a(16)=4, ... */

#include <stdint.h>
#include <stddef.h>

/* Computes the number of biquadratefree divisors of n. */
seq_t a(idx_t n)
{
    uint64_t t = (uint64_t)n;
    seq_t result = 1;

    /* factor 2 */
    if ((t & 1) == 0) {
        uint64_t e = 0;
        while ((t & 1) == 0) {
            t >>= 1;
            ++e;
        }
        result *= ((e <= 3) ? (e + 1) : 4);
    }

    /* odd factors */
    for (uint64_t p = 3; p <= t / p; p += 2) {
        if (t % p == 0) {
            uint64_t e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            result *= ((e <= 3) ? (e + 1) : 4);
        }
    }

    /* remaining prime > sqrt(original n) */
    if (t > 1) {
        /* exponent is 1 => min(1,3)+1 = 2 */
        result *= 2;
    }
    return result;
}
