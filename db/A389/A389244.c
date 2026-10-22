/*
 * A389244  Powerful part of n*(n+1): a(n) = A057521(n*(n+1)).
 * https://oeis.org/A389244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A057521(n*(n+1))
 * Range:     u8 n <= 7, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   build_primes_upto, floor_sqrt, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"
#include "../utils/build_primes_upto.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    seq_t prod = (seq_t)n * ((seq_t)n + 1);
    seq_t res  = 1;

    /* Determine prime limit needed for factorisation */
    idx_t limit = (idx_t)floor_sqrt(prod);

    /* Static cache of primes */
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;
    static idx_t  prime_limit = 0;

    if (limit > prime_limit) {
        free(primes);               /* free old list if any */
        build_primes_upto(limit, &primes, &prime_cnt);
        prime_limit = limit;
    }

    /* Factorise prod and keep only prime powers with exponent ≥ 2 */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        int exp = 0;
        while (prod % p == 0) {
            prod /= p;
            ++exp;
        }
        if (exp >= 2) {
            seq_t part = pow_uint(p, (idx_t)exp);
            res *= part;
        }
    }
    return res;
}
