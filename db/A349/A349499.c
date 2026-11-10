/*
 * A349499  Numbers k such that A349474(k) = A349474(k+1).
 * https://oeis.org/A349499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A349474(k) = A349474(k+1); found by
 *            incremental scan with memoized terms
 * Range:     u8 n <= 6, u16 n <= 649, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9881 (2026-11-10)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

static seq_t A349474_a(idx_t n)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t tau = 1;
    for (size_t i = 0; i < cnt; ++i)
        tau *= ((seq_t)exps[i] + 1);

    seq_t sigma = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t sum = 1;
        seq_t pp = 1;
        seq_t p = (seq_t)primes[i];
        for (uint64_t j = 1; j <= exps[i]; ++j) {
            pp *= p;
            sum += pp;
        }
        sigma *= sum;
    }

    seq_t num = ((seq_t)n) * tau;
    seq_t den = sigma;
    seq_t len = 0;
    while (den != 0) {
        seq_t rem = num % den;
        num = den;
        den = rem;
        ++len;
    }
    return len;
}

seq_t a(idx_t n)
{
    static idx_t *terms = NULL;
    static size_t count = 0, cap = 0;
    static idx_t k = 1;
    static seq_t cur;
    static bool init = false;

    if (!init) {
        cur = A349474_a(1);
        init = true;
    }
    while (count < (size_t)n) {
        seq_t nxt = A349474_a(k + 1);
        if (nxt == cur) {
            if (count == cap) {
                cap = cap ? cap * 2 : 1024;
                terms = (idx_t *)realloc(terms, cap * sizeof(idx_t));
            }
            terms[count++] = k;
        }
        cur = nxt;
        ++k;
    }
    return (seq_t)terms[n - 1];
}
