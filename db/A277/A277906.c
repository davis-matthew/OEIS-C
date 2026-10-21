/*
 * A277906  Least number with same prime signature as phi(n): a(n) =
 *          A046523(A000010(n)).
 * https://oeis.org/A277906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least number with the same prime signature as φ(n)
 * Range:     u8 n <= 125, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, cmp_desc_int, factor_u64, pow_seq, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/calc_phi.h"
#include "../utils/sieve_primes.h"
#include "../utils/factor_u64.h"
#include "../utils/pow_seq.h"
#include "../utils/cmp_desc_int.h"

/* static cache for primes */
static uint64_t *prime_list = NULL;
static size_t prime_cnt = 0;
static bool primes_inited = false;

seq_t a(idx_t n)
{
    /* offset 1: n >= 1 */
    if (n == 0) return (seq_t)0;

    /* initialise prime list once */
    if (!primes_inited) {
        /* enough primes for factoring φ(n) up to ~10^6 (sqrt(10^12)) */
        sieve_primes(20000UL, (idx_t **)&prime_list, &prime_cnt);
        primes_inited = true;
    }

    /* compute φ(n) */
    seq_t phi_val = calc_phi((seq_t)n);
    uint64_t phi = (uint64_t)phi_val;          /* φ(n) fits in 64‑bit for n ≤ 10000 */

    /* factor φ(n) using the uint64_t helper */
    uint64_t pr_small[64];
    uint64_t ex_small[64];
    size_t fac_cnt = 0;
    if (phi > 1) {
        factor_u64(phi, pr_small, ex_small, &fac_cnt);
    }

    /* if φ(n) == 1, its prime signature is empty => result is 1 */
    if (fac_cnt == 0) {
        return (seq_t)1;
    }

    /* copy exponents to int array for sorting */
    int exps[64];
    for (size_t i = 0; i < fac_cnt; ++i) {
        exps[i] = (int)ex_small[i];
    }

    /* sort exponents descending */
    qsort(exps, fac_cnt, sizeof(int), cmp_desc_int);

    /* build minimal number by assigning smallest primes to largest exponents */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < fac_cnt; ++i) {
        seq_t p = (seq_t)prime_list[i];          /* cast to seq_t */
        result = result * pow_seq(p, exps[i]);   /* multiply by p^exp */
    }

    return result;
}
