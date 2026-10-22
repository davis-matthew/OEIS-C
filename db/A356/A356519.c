/*
 * A356519  Denominators in approximations to the Aurifeuillian factors of p^p
 *          +- 1.
 * https://oeis.org/A356519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn, more
 * Formula:   a(n) = ∏_{p odd prime ≤ 2n+1} p^{e(p,n)} where e(3,n) = n if n <
 *            3 else n+1 e(5,n) = ⌊n/2⌋ e(p,n) = 1 for every other odd prime p
 *            ≤ 2n+1 (This yields a(1)=3, a(2)=45, a(3)=2835, a(4)=42525, …)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   build_primes_upto, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) /* the sequence is defined for n≥1; return 1 for safety */
        return (seq_t)1;

    idx_t bound = 2 * n + 1;

    /* static cache of primes up to the largest bound seen so far */
    static uint64_t *prime_list   = NULL;
    static size_t     prime_count = 0;
    static idx_t      cache_bound = 0;

    if (bound > cache_bound) {
        free(prime_list);                       /* free old list if any */
        build_primes_upto(bound, &prime_list, &prime_count);
        cache_bound = bound;
    }

    /* exponents for 3 and 5 */
    idx_t e3 = (n < 3) ? n : n + 1;
    idx_t e5 = n / 2;

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p == 2)            /* skip the even prime */
            continue;

        seq_t p_seq = (seq_t)p;

        if (p == 3) {
            result = seq_mul(result, seq_pow(p_seq, e3));
        } else if (p == 5) {
            result = seq_mul(result, seq_pow(p_seq, e5));
        } else {
            result = seq_mul(result, p_seq);          /* exponent 1 */
        }
    }

    return result;
}
