/*
 * A328465  Row 2 of A328464: a(n) = A276156(4n - 2) / 2.
 * https://oeis.org/A328465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2) * A276156(4*n - 2), where A276156(m) reads the
 *            binary digits of m as digits in primorial base
 * Range:     u8 n <= 8, u16 n <= 32, u32 n <= 512, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 63 terms through n = 7957 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    static seq_t half[66];
    static int initialized = 0;
    if (!initialized) {
        static const unsigned primes[64] = {
            2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,
            59,61,67,71,73,79,83,89,97,101,103,107,109,113,
            127,131,137,139,149,151,157,163,167,173,179,181,
            191,193,197,199,211,223,227,229,233,239,241,251,
            257,263,269,271,277,281,283,293,307,311
        };
        /* half[i] = primorial(i) / 2 for i >= 1 */
        half[0] = (seq_t)0;
        half[1] = (seq_t)1;
        for (idx_t i = 2; i <= 64; ++i) {
            half[i] = half[i-1] * (seq_t)primes[i-1];
        }
        initialized = 1;
    }

    idx_t m = (n << 2) - 2;
    seq_t result = (seq_t)0;
    idx_t pos = 0;
    while (m != 0) {
        if (m & (idx_t)1)
            result += half[pos];
        m >>= 1;
        ++pos;
    }
    return result;
}
