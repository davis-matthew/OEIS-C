/*
 * A259396  Length of runs of identical terms in A080378.
 * https://oeis.org/A259396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the n-th run of equal consecutive terms in
 *            A080378, where A080378(k) = (prime(k+1)-prime(k)) mod 4;
 *            computed with a prime sieve, doubling the limit until the n-th
 *            run is complete
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Validated: partial, pass, 98 terms through n = 98 (2026-11-07)
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    idx_t lim = 1000 + 40 * n;
    for (;;) {
        uint64_t *primes = NULL;
        size_t count = 0;
        build_primes_upto(lim, &primes, &count);
        idx_t runs = 0;
        idx_t len = 0;
        unsigned prev = 5;
        bool found = false;
        idx_t result = 0;
        for (size_t i = 0; i + 1 < count; i++) {
            unsigned v = (unsigned)((primes[i + 1] - primes[i]) % 4);
            if (len == 0) {
                prev = v;
                len = 1;
            } else if (v == prev) {
                len++;
            } else {
                runs++;
                if (runs == n) {
                    result = len;
                    found = true;
                    break;
                }
                prev = v;
                len = 1;
            }
        }
        free(primes);
        if (found) return (seq_t)result;
        lim *= 2;
    }
}
