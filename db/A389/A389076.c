/*
 * A389076  Sum of all other divisors of primeshifted n except its squarefree
 *          part: a(n) = sigma(A003961(n)) - A007913(A003961(n)).
 * https://oeis.org/A389076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003973(n) - A003961(A007913(n)); with n = Product p^e, q
 *            = nextprime(p): a(n) = Product (q^(e+1)-1)/(q-1) - Product_{e
 *            odd} q
 * Range:     u8 n <= 31, u16 n <= 1023, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 93 terms through n = 9496 (2026-11-10)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n) {
    seq_t sig = 1;
    seq_t sqf = 1;
    uint64_t m = (uint64_t)n;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            seq_t q = (seq_t)next_prime(p);
            seq_t term = 1, pw = 1;
            for (uint64_t k = 0; k < e; k++) {
                pw = pw * q;
                term = term + pw;
            }
            sig = sig * term;
            if (e & 1U) sqf = sqf * q;
        }
    }
    if (m > 1) {
        seq_t q = (seq_t)next_prime(m);
        sig = sig * (1 + q);
        sqf = sqf * q;
    }
    return sig - sqf;
}
