/*
 * A355443  a(n) = 1 if n is of the form p^2 * q where p and q are primes with
 *          p^2 < q, otherwise 0.
 * https://oeis.org/A355443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n = p^2 * q with p, q primes and p^2 < q, otherwise
 *            0
 * Range:     u8 n <= 9510, u16 n <= 9510, u32 n <= 9510, u64 n <= 9510,
 *            u128 n <= 9510, big any n
 * Helpers:   p_square_lt_q
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/p_square_lt_q.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;   /* trivial guard */

    uint64_t m = (uint64_t)n;          /* work on 64-bit */
    uint64_t primes[2];
    int exponents[2];
    int pcnt = 0;

    for (uint64_t d = 2; d * d <= m; ++d) {
        if (m % d == 0) {
            int e = 0;
            while (m % d == 0) { m /= d; ++e; }
            primes[pcnt] = d;
            exponents[pcnt] = e;
            ++pcnt;
            if (pcnt > 2) return 0;   /* more than two distinct primes */
        }
    }
    if (m > 1) {
        primes[pcnt] = m;
        exponents[pcnt] = 1;
        ++pcnt;
        if (pcnt > 2) return 0;
    }
    if (pcnt != 2) return 0;

    /* ensure exponents are {2,1} */
    if ((exponents[0] == 2 && exponents[1] == 1) ||
        (exponents[0] == 1 && exponents[1] == 2)) {

        uint64_t p = (exponents[0] == 2 ? primes[0] : primes[1]);
        uint64_t q = (exponents[0] == 2 ? primes[1] : primes[0]);

        if (p_square_lt_q(p, q)) {
            return 1;
        }
    }
    return 0;
}
