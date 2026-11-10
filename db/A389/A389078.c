/*
 * A389078  Primeshifted n minus its squarefree part: a(n) = A003961(n) -
 *          A007913(A003961(n)).
 * https://oeis.org/A389078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003961(n) - A007913(A003961(n)); A003961 is completely
 *            multiplicative with a(prime(k)) = prime(k+1), so this is the
 *            product of q^e minus the product of q over odd e, where q =
 *            nextprime(p) for each p^e exactly dividing n
 * Range:     u8 n <= 47, u16 n <= 1535, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9945 (2026-11-10)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n) {
    seq_t full = 1;
    seq_t sqf = 1;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            seq_t q = (seq_t)next_prime(p);
            int odd = 0;
            while (m % p == 0) {
                m /= p;
                full *= q;
                odd ^= 1;
            }
            if (odd) sqf *= q;
        }
    }
    if (m > 1) {
        seq_t q = (seq_t)next_prime(m);
        full *= q;
        sqf *= q;
    }
    return full - sqf;
}
