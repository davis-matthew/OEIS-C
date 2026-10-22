/*
 * A387942  In the prime factorization of n, replace each prime with the
 *          number of nonzero digits in its balanced ternary representation.
 * https://oeis.org/A387942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, mult, easy
 * Formula:   a(n) = Π_{p^e || n} (A005812(p))^e, where A005812(p) = number of
 *            non‑zero digits in the balanced ternary representation of the
 *            prime p
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bt_nonzero_digit_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function implements a totally multiplicative definition
   without using the special helper libraries. */

#include <stdint.h>
#include "../utils/bt_nonzero_digit_count.h"

seq_t a(idx_t n)
{
    seq_t result = 1;
    if (n < 1) return result;    /* undefined for n < 1; return 1 */

    uint64_t m = n;              /* work on a 64‑bit copy of n */

    while (m > 1U) {
        uint64_t p;

        /* find a prime factor of m */
        if ((m & 1U) == 0U) {
            p = 2U;
        } else {
            uint64_t i;
            for (i = 3U; i * i <= m; i += 2U) {
                if (m % i == 0U) {
                    p = i;
                    break;
                }
            }
            if (i * i > m) {
                p = m;            /* m is prime */
            }
        }

        /* exponent of p in m */
        uint64_t e = 0U;
        while (m % p == 0U) {
            m /= p;
            ++e;
        }

        /* multiply the contribution of this prime factor */
        seq_t cnt = bt_nonzero_digit_count(p);
        seq_t pow_val = 1;
        while (e > 0U) {
            pow_val *= cnt;
            --e;
        }
        result *= pow_val;
    }

    return result;
}
