/*
 * A370297  Inverse Moebius transform of A322328.
 * https://oeis.org/A370297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} mu(d)^2 * tau(n/d)^2 (equivalently a(p^e) = 1 +
 *            2*e*(e+1) for each prime power) This implementation factors n
 *            and multiplies the local formulas
 * Range:     u8 n <= 59, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* out of range, but harmless */

    uint64_t temp = (uint64_t)n;
    seq_t result = 1;

    /* factor 2 */
    uint64_t cnt = 0;
    while ((temp & 1ULL) == 0) {
        ++cnt;
        temp >>= 1;
    }
    if (cnt) {
        seq_t term = 1 + (seq_t)2 * (seq_t)cnt * (cnt + 1);
        result *= term;
    }

    /* odd prime factors */
    for (uint64_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            uint64_t c = 0;
            while (temp % p == 0) {
                temp /= p;
                ++c;
            }
            seq_t term = 1 + (seq_t)2 * (seq_t)c * (c + 1);
            result *= term;
        }
    }

    /* remaining prime factor, if any */
    if (temp > 1) {
        seq_t term = 1 + (seq_t)2 * (seq_t)1 * (1 + 1); /* e = 1 */
        result *= term;
    }

    return result;
}
