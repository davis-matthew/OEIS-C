/*
 * A379507  For n >= 1, a(n) = GCD((2*n)! / (n!)^2, 105).
 * https://oeis.org/A379507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = GCD((2*n)!/(n!)^2, 105)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* the primes that divide 105 */
    const uint64_t primes[3] = {3, 5, 7};
    seq_t result = 1;

    /* cast to 64‑bit unsigned for work */
    uint64_t N = n;

    /* 2*n as a 128‑bit value to avoid overflow */
    unsigned __int128 twoN = (unsigned __int128)N * 2;

    for (int i = 0; i < 3; ++i) {
        uint64_t p = primes[i];

        unsigned long long vp_num = 0;
        unsigned long long vp_den = 0;

        /* compute v_p((2n)!) */
        unsigned __int128 power = p;
        while (power <= twoN) {
            vp_num += (unsigned long long)(twoN / power);
            power *= p;
        }

        /* compute v_p(n!) and double it for the denominator */
        power = p;
        while (power <= N) {
            vp_den += (unsigned long long)(N / power);
            power *= p;
        }
        vp_den <<= 1;  /* multiply by 2 */

        if (vp_num > vp_den) {
            /* this prime divides the central binomial coefficient */
            result *= (seq_t)p;
        }
    }

    return result;
}
