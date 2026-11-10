/*
 * A277434  Primes in A001075.
 * https://oeis.org/A277434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard
 * Formula:   a(n) = n-th prime among A001075(2^k), where A001075(1) = 2 and
 *            A001075(2m) = 2*A001075(m)^2 - 1; found by testing k = 0..4 (no
 *            further terms are known; a(5), if it exists, is at least
 *            A001075(2^23))
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 4 terms through n = 4 (2026-11-08)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n) {
    seq_t x = 2;          /* A001075(2^0) */
    idx_t found = 0;
    int k;
    if (n == 0) return 0;
    for (k = 0; k <= 4; k++) {
        if (k > 0) x = 2 * x * x - 1;   /* A001075(2^k) */
        if (is_prime_seq(x)) {
            found++;
            if (found == n) return x;
        }
    }
    return 0; /* no further terms known */
}
