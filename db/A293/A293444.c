/*
 * A293444  a(n) = A293442(A293442(n)), where A293442 is multiplicative with
 *          a(p^e) = A019565(e).
 * https://oeis.org/A293444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A293442(A293442(n)), where A293442 is multiplicative with
 *            a(p^e) = A019565(e), i.e. the product of prime(k+1) over the set
 *            bits k of e
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9784 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A293442_f(uint64_t n)
{
    static const uint32_t pr[16] = {2, 3, 5, 7, 11, 13, 17, 19,
                                    23, 29, 31, 37, 41, 43, 47, 53};
    uint64_t res = 1;
    uint64_t p;
    for (p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            unsigned e = 0;
            unsigned k;
            uint64_t t = 1;
            while (n % p == 0) { n /= p; ++e; }
            for (k = 0; e != 0; ++k, e >>= 1)
                if (e & 1u) t *= pr[k];
            res *= t;
        }
    }
    if (n > 1) res *= 2; /* exponent 1 -> A019565(1) = 2 */
    return res;
}

seq_t a(idx_t n)
{
    uint64_t m = A293442_f((uint64_t)n);
    return (seq_t)A293442_f(m);
}
