/*
 * A320200  Number of integer solutions to a^2 + 2*b^2 + 4*c^2 + 12*d^2 = n.
 * https://oeis.org/A320200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Number of integer solutions to * a^2 + 2*b^2 + 4*c^2 + 12*d^2 =
 *            n. * The implementation enumerates all non‑negative values of
 *            a,b,c,d * and multiplies by the appropriate sign multiplicities
 * Range:     u8 n <= 129, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


/* Compute floor(sqrt(v)) for an unsigned 64‑bit integer. */
static inline idx_t isqrt(idx_t v)
{
    idx_t res = 0;
    idx_t bit = 1ULL << 62;            /* highest power of 4 <= 2^64-1 */
    while (bit > v) bit >>= 2;
    while (bit) {
        if (v >= res + bit) {
            v -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

/* Count solutions to a^2 + 2*b^2 + 4*c^2 = m.  a,b,c are integers. */
static seq_t reps_a2_2b2_4c2(idx_t m)
{
    seq_t total = 0;

    for (idx_t c = 0; ; ++c) {
        __uint128_t c2 = (__uint128_t)c * c;
        __uint128_t foursq = c2 * 4;
        if (foursq > m) break;

        idx_t rest_m = m - (idx_t)foursq;
        seq_t count_ab = 0;

        for (idx_t b = 0; ; ++b) {
            __uint128_t b2 = (__uint128_t)b * b;
            __uint128_t twosq = b2 * 2;
            if (twosq > rest_m) break;

            idx_t r = rest_m - (idx_t)twosq;
            idx_t a = isqrt((idx_t)r);
            if ((a * (idx_t)a) == r) {
                if (b == 0) {
                    count_ab += (a == 0) ? 1 : 2;
                } else {
                    count_ab += (a == 0) ? 2 : 4;   /* b sign */
                }
            }
        }

        if (c == 0) total += count_ab;
        else       total += 2 * count_ab;           /* c sign */
    }

    return total;
}

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t d = 0; ; ++d) {
        __uint128_t d2 = (__uint128_t)d * d;
        __uint128_t thirteensq = d2 * 12;
        if (thirteensq > n) break;

        idx_t m = n - (idx_t)thirteensq;
        seq_t rep = reps_a2_2b2_4c2(m);
        if (d == 0) result += rep;
        else        result += 2 * rep;               /* d sign */
    }
    return result;
}
