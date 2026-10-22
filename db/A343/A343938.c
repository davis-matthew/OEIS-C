/*
 * A343938  Twice the number of prime factors of n minus the sum of prime
 *          indices of n, both counted with multiplicity.
 * https://oeis.org/A343938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*Ω(n) - Σ e * index(p) over prime factorisation of n
 * Range:     i8 n <= 738, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64, build_primes_upto, compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/build_primes_upto.h"
#include "../utils/compute_pi.h"

static idx_t prime_limit = 0;
static uint64_t *prime_list = NULL;
static size_t prime_cnt = 0;

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* undefined for n < 1 */

    /* Ensure we have all primes up to sqrt(n) for factorisation. */
    idx_t sq = _floor_isqrt_u64(n);
    if (sq > prime_limit) {
        free(prime_list);
        build_primes_upto(sq, &prime_list, &prime_cnt);
        prime_limit = sq;
    }

    uint64_t tmp = n;
    seq_t total_mult  = 0;   /* Ω(n) */
    seq_t sum_indices = 0;   /* Σ e * index(p) */

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if ((idx_t)p * p > tmp) break;
        if (tmp % p != 0) continue;

        uint64_t e = 0;
        do {
            tmp /= p;
            ++e;
        } while (tmp % p == 0);

        total_mult  += (seq_t)e;
        sum_indices += (seq_t)e * (seq_t)(i + 1);  /* index is 1‑based */
    }

    if (tmp > 1) {                     /* remaining prime factor */
        total_mult  += 1;
        sum_indices += compute_pi(tmp);  /* index of the prime */
    }

    return 2 * total_mult - sum_indices;
}
