/*
 * A278110  a(n) = Product_{k=1..A056811(n)} A000040(k)^A278109(n,k).
 * https://oeis.org/A278110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{p prime} p^{⌊ n / p² ⌋} (product over primes with p² ≤
 *            n)
 * Range:     u8 n <= 19, u16 n <= 35, u32 n <= 62, u64 n <= 116,
 *            u128 n <= 215, big any n
 * Helpers:   _floor_isqrt_u64, big_pow, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/build_primes_upto.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 1) {
        return (seq_t)1;
    }

    /* Upper bound for primes: floor(sqrt(n)) */
    idx_t limit = _floor_isqrt_u64(n);

    /* Generate all primes ≤ limit */
    uint64_t *primes = NULL;
    size_t count = 0;
    build_primes_upto(limit, &primes, &count);

    /* Compute product */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < count; ++i) {
        idx_t p   = primes[i];
        idx_t pp  = p * p;                /* p² – fits in 64 bit for n ≤ 10⁴ */
        idx_t e   = n / pp;                /* exponent floor(n / p²) */
        if (e) {
            seq_t pow_val = big_pow((seq_t)p, (seq_t)e);
            result *= pow_val;
        }
    }

    /* Clean up */
    free(primes);
    return result;
}
