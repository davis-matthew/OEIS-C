/*
 * A320114  a(n) = Product_{d|n, d>1} prime(1+(d mod 4)).
 * https://oeis.org/A320114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n, d>1} prime(1+(d mod 4))
 * Range:     u8 n <= 11, u16 n <= 29, u32 n <= 209, u64 n <= 1259,
 *            u128 n <= 8192, big any n
 * Helpers:   _product_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_product_divisors.h"

seq_t a(idx_t n)
{
    uint64_t tn = (uint64_t)n;
    uint64_t primes[20];
    int exp[20];
    int cnt = 0;

    /* factor 2 */
    if (tn % 2 == 0) {
        int e = 0;
        while (tn % 2 == 0) {
            tn /= 2;
            ++e;
        }
        primes[cnt] = 2;
        exp[cnt] = e;
        ++cnt;
    }

    /* factor odd primes */
    for (uint64_t p = 3; p <= tn / p; p += 2) {
        if (tn % p == 0) {
            int e = 0;
            while (tn % p == 0) {
                tn /= p;
                ++e;
            }
            primes[cnt] = p;
            exp[cnt] = e;
            ++cnt;
        }
    }
    if (tn > 1) {
        primes[cnt] = tn;
        exp[cnt] = 1;
        ++cnt;
    }

    seq_t result = 1;
    _product_divisors(0, primes, exp, cnt, 1, &result);
    return result;
}
