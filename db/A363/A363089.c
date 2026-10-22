/*
 * A363089  Positive numbers k for which cos(k) > sin(k).
 * https://oeis.org/A363089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 126, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: integers k with cos(k) > sin(k) computed via Beatty‑interval method */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Rational approximation of 2π with denominator 10^18 */
    const int64_t N64 = 6283185307179586476LL; /* floor(2π * 10^18) */
    const int64_t C1  = -2356194490192345000LL; /* floor(-3π/4 * 10^18) */
    const int64_t C2  =  785398163397448300LL; /* floor( π/4  * 10^18) */
    const __int128 D  = 1000000000000000000LL; /* 10^18 */

    uint64_t count = 0;
    unsigned long long m = 1; /* m >= 1 corresponds to interval ( -3π/4+2πm , π/4+2πm ) */

    while (1) {
        __int128 numL = (__int128)N64 * m + (__int128)C1;     /* floor(2πm - 3π/4) * 10^18 */
        __int128 floorL = numL / D;          /* floor(2πm - 3π/4) */

        __int128 numU = (__int128)N64 * m + (__int128)C2;     /* floor(2πm + π/4) * 10^18 */
        __int128 floorU = numU / D;          /* floor(2πm + π/4) */

        uint64_t groupSize = (uint64_t)(floorU - floorL); /* number of integers in this group */

        if (count + groupSize >= n) {
            /* falling inside current group */
            __int128 val = floorL + (n - count); /* floorL+1 + (n-count-1), simplified */
            return (seq_t)val;
        }

        count += groupSize;
        ++m;
    }
}
