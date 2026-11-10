/*
 * A373372  a(n) = 1 if A001414(n) and A276085(n) are both multiples of 3,
 *          otherwise 0, where A001414 is the sum of prime factors with
 *          repetition and A276085 is the primorial base log-function.
 * https://oeis.org/A373372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [A001414(n) == 0 (mod 3)] * [v_2(n) == v_3(n) (mod 3)]
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 141 terms through n = 9823 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    unsigned e2 = 0, e3 = 0;
    uint64_t sum3 = 0; /* sum of prime factors with repetition, mod 3 */

    while ((m & 1) == 0 && m != 0) { m >>= 1; e2++; sum3 += 2; }
    while (m % 3 == 0 && m != 0) { m /= 3; e3++; }
    /* each factor 3 adds 0 mod 3 */

    for (uint64_t p = 5; p <= m / p; p += 2) {
        while (m % p == 0) {
            m /= p;
            sum3 += p % 3;
        }
    }
    if (m > 1) sum3 += m % 3;

    if ((sum3 % 3) != 0) return (seq_t)0;
    return (seq_t)(((e2 % 3) == (e3 % 3)) ? 1 : 0);
}
