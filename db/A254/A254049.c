/*
 * A254049  Odd bisection of A048673: a(n) = A048673(2*n-1).
 * https://oeis.org/A254049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A048673(2*n-1) = (1 + A249735(n))/2
 * Range:     u8 n <= 31, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   build_primes, factor_u64, get_prime, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/get_prime.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"
#include "../utils/build_primes.h"

seq_t a(idx_t n)
{
    static bool init = false;
    static idx_t *prime_list = NULL;
    static size_t prime_cnt = 0;

    if (!init) {
        const idx_t LIMIT = (idx_t)40000;   /* enough for all needed odd numbers up to 2*10^4 */
        build_primes(LIMIT, &prime_list, &prime_cnt);
        init = true;
    }

    /* m = 2*n - 1 (odd number whose prime factors are to be shifted) */
    uint64_t m = (uint64_t)2 * n - 1;

    /* factor m */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64(m, pr, ex, &cnt);

    seq_t product = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t factor = pr[i];
        unsigned int exp = (unsigned int)ex[i];

        /* find 1‑based index of factor in the prime list */
        size_t lo = 0, hi = prime_cnt;
        size_t idx = 0;          /* will become 1‑based index */
        while (lo < hi) {
            size_t mid = (lo + hi) >> 1;
            if (prime_list[mid] == factor) {
                idx = mid + 1;   /* 1‑based */
                break;
            }
            if (prime_list[mid] < factor)
                lo = mid + 1;
            else
                hi = mid;
        }
        if (idx == 0) continue; /* should not happen */

        /* next prime after the factor */
        seq_t next_prime = (seq_t)get_prime(idx + 1);   /* idx+1 is the next index */

        /* multiply by (next_prime)^exp */
        seq_t pow_val = seq_pow(next_prime, exp);
        product = seq_mul(product, pow_val);
    }

    /* (product + 1) / 2 gives the final term */
    seq_t result = (product + (seq_t)1) / (seq_t)2;
    return result;
}
