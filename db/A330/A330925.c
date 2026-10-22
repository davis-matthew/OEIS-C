/*
 * A330925  For any n >= 0: consider all pairs of numbers (x, y) whose binary
 *          representations can be interleaved (or shuffled) to produce the
 *          binary representation of n (possibly with leading zeros); a(n) is
 *          the greatest possible value of x*y.
 * https://oeis.org/A330925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = max{ x*y : x,y interleaving of binary(n) }, with
 *            recurrence a(2*n) = 2*a(n). For n = 0 or a power of 2, a(n) = 0
 * Range:     u8 n <= 302, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* base case */
    if (n == 0) {
        return 0;
    }
    /* use the recurrence for even n */
    if ((n & 1) == 0) {
        return 2 * a(n / 2);
    }

    /* n is odd – enumerate all partitions of its bits */
    int L = 0;
    idx_t tmp = n;
    while (tmp) {
        L++;
        tmp >>= 1;
    }

    /* extract bits of n from most‑significant to least‑significant */
    uint8_t bits[64];
    tmp = n;
    for (int i = L - 1; i >= 0; --i) {
        bits[i] = (uint8_t)(tmp & 1);
        tmp >>= 1;
    }

    seq_t best = 0;
    uint64_t mask_limit = 1ULL << L;
    for (uint64_t mask = 0; mask < mask_limit; ++mask) {
        seq_t x = 0;
        seq_t y = 0;
        for (int i = 0; i < L; ++i) {
            uint8_t bit = bits[i];
            if ((mask >> i) & 1ULL) {   /* bit goes to x */
                x = (x << 1) | (seq_t)bit;
            } else {                    /* bit goes to y */
                y = (y << 1) | (seq_t)bit;
            }
        }
        seq_t prod = x * y;
        if (prod > best) {
            best = prod;
        }
    }
    return best;
}
