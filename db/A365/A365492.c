/*
 * A365492  The number of divisors of the smallest 4th power divisible by n.
 * https://oeis.org/A365492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product_{p^e || n} (4*ceil(e/4)+1) (sequence A365492)
 * Range:     u8 n <= 209, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* index 0 is outside the offset */

    uint64_t m = n;
    seq_t res = 1;

    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            uint64_t ceil_e4 = (e + 3) / 4;        /* ceil(e/4) */
            res *= (seq_t)(4 * ceil_e4 + 1);
        }
    }

    if (m > 1) {          /* remaining prime factor with exponent 1 */
        uint64_t e = 1;
        uint64_t ceil_e4 = (e + 3) / 4;           /* = 1 */
        res *= (seq_t)(4 * ceil_e4 + 1);          /* = 5 */
    }

    return res;
}
