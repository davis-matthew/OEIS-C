/*
 * A322980  a(n) = 1 if n and d(n) are coprime, 0 otherwise. Here d(n) is the
 *          number of divisors of n, A000005.
 * https://oeis.org/A322980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if gcd(n, d(n)) == 1, else 0, where d(n) is the number
 *            of divisors of n
 * Range:     u8 n <= 8407, u16 n <= 8407, u32 n <= 8407, u64 n <= 8407,
 *            u128 n <= 8407, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline unsigned long long gcd_ull(unsigned long long a,
                                         unsigned long long b) {
    while (b) {
        unsigned long long t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    if (n == 0)  /* undefined for the sequence, but return 0 */
        return (seq_t)0;

    unsigned long long nn = n;               /* local copy for factorisation */
    unsigned long long d   = 1;              /* divisor count */

    for (unsigned long long p = 2; p <= nn / p; ++p) {
        if (nn % p == 0) {
            unsigned long long exp = 0;
            while (nn % p == 0) {
                nn /= p;
                ++exp;
            }
            d *= (exp + 1);
        }
    }
    if (nn > 1)          /* remaining prime factor */
        d *= 2;

    unsigned long long g = gcd_ull(n, d);
    return (g == 1) ? (seq_t)1 : (seq_t)0;
}
