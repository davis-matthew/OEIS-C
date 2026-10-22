/*
 * A384009  Irregular triangle read by rows where row n lists the positive
 *          first differences of the prime indices of n.
 * https://oeis.org/A384009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) is the n‑th positive first difference of the sorted list
   of prime indices of integers, read by rows. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Static data for the prime sieve and the index table. */
    static uint64_t *prime_arr  = NULL;
    static size_t   prime_cnt  = 0;
    static size_t   prime_limit = 0;
    static uint32_t *prime_index = NULL;

    /* Lazily initialise on the first call. */
    if (prime_arr == NULL) {
        /* A limit large enough for all primes that can appear as factors of
           integers up to the expected maximum (2 000 000 is more than enough). */
        prime_limit = 200000;  /* 200 k > 10⁴ and gives ≈ 18 k primes. */
        build_primes_upto((idx_t)prime_limit, &prime_arr, &prime_cnt);

        /* Build a lookup table: prime_value → 1‑based prime index. */
        prime_index = (uint32_t*)calloc(prime_limit + 1, sizeof(uint32_t));
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t idx = (uint32_t)(i + 1);
            prime_index[(size_t)prime_arr[i]] = idx;
        }
    }

    /* Buffers for factorisation and for the list of prime indices. */
    uint64_t primes_f[64];
    uint64_t exps_f[64];
    size_t cnt_f;

    uint32_t indices[64];
    size_t idx_cnt;

    size_t produced = 0;   /* how many positive differences have been emitted */

    /* Enumerate integers starting from 2 and emit positive first differences
       until the n‑th one is reached. */
    for (uint64_t num = 2; ; ++num) {
        factor_u64(num, primes_f, exps_f, &cnt_f);

        idx_cnt = 0;
        for (size_t i = 0; i < cnt_f; ++i) {
            uint32_t idx = prime_index[(size_t)primes_f[i]];
            for (size_t k = 0; k < exps_f[i]; ++k) {
                indices[idx_cnt++] = idx;
            }
        }

        for (size_t j = 1; j < idx_cnt; ++j) {
            uint32_t diff = indices[j] - indices[j-1];
            if (diff > 0) {
                ++produced;
                if (produced == (size_t)n) {
                    return (seq_t)diff;
                }
            }
        }
    }
}
