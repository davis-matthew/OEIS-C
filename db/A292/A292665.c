/*
 * A292665  Rank of n*(e-1) when all the numbers j*(e-1) and k*(e+1), for
 *          j>=1, k>=1, are jointly ranked.
 * https://oeis.org/A292665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(n*(e-1)/(e+1))
 * Range:     u8 n <= 175, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    static bool initialized = false;
    static const uint64_t B = 2432902008176640000ULL; /* 20! */
    static uint64_t A = 0;
    static uint64_t num = 0;
    static uint64_t den = 0;

    if (!initialized) {
        uint64_t cur = B;
        A = 0;
        for (int k = 0; k <= 20; ++k) {
            A += cur;
            if (k < 20) cur /= (uint64_t)(k + 1);
        }
        num = A - B;            /* numerator of (e-1)/(e+1) */
        den = A + B;            /* denominator */
        initialized = true;
    }

    unsigned __int128 prod = (unsigned __int128)n * num;
    uint64_t floor_val = (uint64_t)(prod / den);   /* floor(n * r) */

    return (seq_t)(n + (seq_t)floor_val);
}
