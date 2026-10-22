/*
 * A394435  Primes p such that A001414(p-1)^2 + A001414(p+1)^2 is a perfect
 *          square, where A001414(n) is the sum of prime factors of n with
 *          multiplicity (sopfr).
 * https://oeis.org/A394435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 9, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   ensure_capacity, factorize, floor_sqrt, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A394435: Primes p such that 
   A001414(p-1)^2 + A001414(p+1)^2 is a perfect square.
   Where A001414(n) = sum of prime factors of n with multiplicity
*/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/factorize.h"    /* size_t factorize(uint64_t n, uint64_t *, int *); */
#include "../utils/floor_sqrt.h"  /* seq_t floor_sqrt(seq_t); */
#include "../utils/next_prime.h"  /* uint64_t next_prime(uint64_t); */
#include "../utils/ensure_capacity.h" /* void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size); */

static seq_t sopfr_uint64(uint64_t n)
{
    if (n <= 1) return 0;
    uint64_t primes[64];
    int exps[64];
    size_t cnt = factorize(n, primes, exps);
    seq_t sum = 0;
    for (size_t i = 0; i < cnt; i++) {
        sum += (seq_t)primes[i] * (seq_t)exps[i];
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but avoid UB */

    /* static cache of discovered primes */
    static seq_t *terms = NULL;
    static size_t terms_cap = 0;
    static size_t terms_cnt = 0;
    static uint64_t last_prime = 0;  /* last prime examined */

    while (terms_cnt < n) {
        /* find next prime */
        if (last_prime == 0)
            last_prime = 2;
        else
            last_prime = next_prime(last_prime + 1);

        seq_t a_val = sopfr_uint64((uint64_t)last_prime - 1);
        seq_t b_val = sopfr_uint64((uint64_t)last_prime + 1);

        seq_t sq = a_val * a_val + b_val * b_val;
        seq_t c = floor_sqrt(sq);
        if (c * c == sq) {
            /* store term */
            ensure_capacity(terms_cnt + 1, &terms_cap,
                            (void **)&terms, sizeof(seq_t));
            terms[terms_cnt++] = last_prime;
        }
    }

    return terms[n - 1];
}
