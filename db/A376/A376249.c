/*
 * A376249  Numbers that are not prime powers and have a unique largest prime
 *          exponent that is larger than the second-largest prime exponent by
 *          1.
 * https://oeis.org/A376249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, ensure_capacity, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A376249: numbers that are not prime powers and whose
 * unordered prime exponent multiset ends with two consecutive integers.
 * The function enumerates integers, tests the factor exponents, and
 * stores found terms in a static array so that subsequent calls are
 * fast, regardless of call order.  */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/factor_u64.h"
#include "../utils/build_primes_upto.h"
#include "../utils/ensure_capacity.h"

static seq_t *seq = NULL;       /* cached sequence values */
static size_t seq_sz = 0;       /* current number of terms stored */
static size_t seq_cap = 0;      /* capacity of seq array */
static uint64_t candidate = 2;  /* next integer to test */

static uint64_t *prime_tbl = NULL;   /* prime sieve */
static size_t prime_cnt = 0;
static bool prime_init = false;

/* Test whether n satisfies the definition of A376249. */
static bool is_desired(uint64_t n)
{
    if (!prime_init) {
        /* Build a sufficient prime table once. */
        build_primes_upto(2000000, &prime_tbl, &prime_cnt);
        prime_init = true;
    }

    /* Factor n.  We only need exponents. */
    static uint64_t exps[64];
    size_t fac_cnt = 0;
    factor_u64(n, prime_tbl, exps, &fac_cnt);

    if (fac_cnt < 2) return false;          /* need at least two primes */

    uint64_t max1 = 0, max2 = 0;
    size_t cnt_max1 = 0;
    for (size_t i = 0; i < fac_cnt; ++i) {
        uint64_t e = exps[i];
        if (e > max1) {
            max2 = max1;
            max1 = e;
            cnt_max1 = 1;
        } else if (e == max1) {
            ++cnt_max1;
        } else if (e > max2) {
            max2 = e;
        }
    }

    if (cnt_max1 != 1) return false;        /* largest exponent must be unique */
    return (max1 == max2 + 1);
}

seq_t a(idx_t n)
{
    if (seq == NULL) {
        seq = NULL;
        seq_sz = 0;
        seq_cap = 0;
        candidate = 2;
    }

    while (seq_sz < (size_t)n) {
        while (!is_desired(candidate)) ++candidate;

        ensure_capacity(seq_sz + 1, &seq_cap, (void **)&seq, sizeof(seq_t));
        seq[seq_sz++] = (seq_t)candidate;
        ++candidate;
    }
    return seq[n - 1];
}
