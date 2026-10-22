/*
 * A332464  Rule 124 one-dimensional cellular automaton applied for one step
 *          to the configuration read from the base-2 expansion of sigma(n),
 *          then converted back to decimal.
 * https://oeis.org/A332464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = result of applying Rule 124 cellular automaton to the
 *            binary representation of σ(n) (sum of divisors of n),
 *            interpreted as a decimal integer
 * Range:     u8 n <= 48, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ----- compute σ(n) ----- */
    seq_t sigma = 1;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            seq_t pow = 1;
            while (m % p == 0) {
                m /= p;
                pow = pow * (seq_t)p;
            }
            seq_t numer = pow * (seq_t)p - 1;
            seq_t denom = (seq_t)p - 1;
            seq_t sigma_p = numer / denom;
            sigma = sigma * sigma_p;
        }
    }
    if (m > 1) {
        seq_t p = (seq_t)m;
        sigma = sigma * (p + 1);
    }

    /* ----- binary length of sigma ----- */
    unsigned len = 0;
    seq_t temp = sigma;
    while (temp) { temp >>= 1; ++len; }
    if (len == 0) len = 1;          /* for safety, sigma cannot be zero */

    /* ----- extract bits (most‑significant first) ----- */
    uint8_t *bits = (uint8_t *)malloc(len * sizeof(uint8_t));
    if (!bits) return 0;             /* out of memory fallback */

    for (unsigned i = 0; i < len; ++i) {
        unsigned shift = len - 1 - i;
        bits[i] = (uint8_t)((sigma >> shift) & 1);
    }

    /* ----- Rule 124 mapping ----- */
    static const uint8_t map[8] = {0,0,1,1,1,1,1,0};
    seq_t result = 0;
    unsigned newlen = len + 1;

    for (unsigned i = 0; i < newlen; ++i) {
        uint8_t left   = (i > 0 && i - 1 < len) ? bits[i - 1] : 0;
        uint8_t center = (i < len) ? bits[i] : 0;
        uint8_t right  = (i + 1 < len) ? bits[i + 1] : 0;
        unsigned idx   = (left << 2) | (center << 1) | right;
        uint8_t newbit = map[idx];
        result = (result << 1) | newbit;
    }

    free(bits);
    return result;
}
