/*
 * A356497  a(n) = maximal 2^k such that there exists a (2^k)-th root of unity
 *          modulo n.
 * https://oeis.org/A356497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximal 2^k such that there exists a (2^k)-th root of
 *            unity modulo n
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n <= 1) {
        return 1;
    }

    idx_t temp = n;
    uint8_t maxk = 0;

    /* factor 2 */
    if ((temp & 1ULL) == 0) {
        uint8_t e = 0;
        while ((temp & 1ULL) == 0) {
            temp >>= 1;
            e++;
        }
        uint8_t k;
        if (e == 1) {
            k = 0;
        } else if (e == 2) {
            k = 1;
        } else {
            k = e - 2;
        }
        if (k > maxk) maxk = k;
    }

    /* factor odd primes by trial division */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            /* read exponent but actually irrelevant for odd primes */
            while (temp % p == 0) {
                temp /= p;
            }
            /* v2(p-1) */
            idx_t v = p - 1;
            uint8_t k = 0;
            while ((v & 1ULL) == 0) {
                v >>= 1;
                k++;
            }
            if (k > maxk) maxk = k;
        }
    }

    /* remaining prime factor > sqrt(original n) */
    if (temp > 1) {
        idx_t v = temp - 1;
        uint8_t k = 0;
        while ((v & 1ULL) == 0) {
            v >>= 1;
            k++;
        }
        if (k > maxk) maxk = k;
    }

    return pow2((idx_t)maxk);
}
