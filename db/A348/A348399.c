/*
 * A348399  a(n) = Sum_{d|n} sigma_[d](n), where sigma_[k](n) is the sum of
 *          the k-th powers of the divisors of n.
 * https://oeis.org/A348399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} sigma_[d](n) where sigma_[k](n) = sum_{e|n} e^k
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/big_pow.h"

/* Helper: recursively generate all divisors from prime factorization */
static void gen_divs_rec(const uint64_t *primes, const size_t *exps,
                         size_t num_factors, size_t pos, idx_t curr,
                         idx_t *array, size_t *idx) {
    if (pos == num_factors) {
        array[*idx] = curr;
        (*idx)++;
        return;
    }
    uint64_t p = primes[pos];
    size_t exp = exps[pos];
    idx_t val = curr;
    for (size_t e = 0; e <= exp; ++e) {
        gen_divs_rec(primes, exps, num_factors, pos + 1, val, array, idx);
        val *= (idx_t)p;
    }
}

/* Compute all divisors of n into *out (array) and count into *cnt */
static void get_divisors(idx_t n, idx_t **out, size_t *cnt) {
    uint64_t tmp = (uint64_t)n;
    uint64_t primes[16];
    size_t exps[16];
    size_t fcount = 0;

    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            size_t c = 0;
            while (tmp % p == 0) {
                tmp /= p;
                c++;
            }
            primes[fcount] = p;
            exps[fcount] = c;
            fcount++;
        }
    }
    if (tmp > 1) {
        primes[fcount] = tmp;
        exps[fcount] = 1;
        fcount++;
    }

    size_t total = 1;
    for (size_t i = 0; i < fcount; ++i)
        total *= (exps[i] + 1);

    idx_t *arr = (idx_t *)malloc(total * sizeof(idx_t));
    size_t index = 0;
    gen_divs_rec(primes, exps, fcount, 0, (idx_t)1, arr, &index);

    *out = arr;
    *cnt = total;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* offset starts at 1, behaviour below 1 is irrelevant */

    idx_t *divs;
    size_t div_cnt;
    get_divisors(n, &divs, &div_cnt);

    seq_t result = 0;
    for (size_t i = 0; i < div_cnt; ++i) {
        seq_t e = (seq_t)divs[i];
        seq_t inner = 0;
        for (size_t j = 0; j < div_cnt; ++j) {
            inner = inner + big_pow(e, (seq_t)divs[j]);
        }
        result = result + inner;
    }

    free(divs);
    return result;
}
