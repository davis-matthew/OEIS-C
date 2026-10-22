/*
 * A331627  Integers that are (exactly) k-deficient-perfect numbers.
 * https://oeis.org/A331627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence: Integers that are exactly k‑deficient‑perfect numbers.
 *  Computed by testing each integer sequentially for the property.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/sigma_of.h"
#include "../utils/factor_u64.h"

static seq_t *terms = NULL;      /* array of found terms */
static size_t terms_len = 0;     /* number of terms stored */
static size_t terms_cap = 0;     /* capacity of the array */
static idx_t tested = 0;         /* largest integer examined */

/* append a term to the array, growing capacity if necessary */
static void append_term(seq_t val) {
    if (terms_len == terms_cap) {
        terms_cap = terms_cap ? terms_cap * 2 : 16;
        seq_t *p = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
        if (!p) abort();   /* out of memory */
        terms = p;
    }
    terms[terms_len++] = val;
}

/* main sequence function */
seq_t a(idx_t n) {
    /* Ensure the array has at least n terms */
    while (terms_len < n) {
        ++tested;
        uint64_t u = (uint64_t)tested;   /* current integer to test */

        /* 1 is a special case */
        if (tested == 1) {
            append_term((seq_t)1);
            continue;
        }

        /* Calculate sum of divisors and check for deficiency */
        seq_t sigma = sigma_of(tested);
        seq_t S = sigma - (seq_t)tested;          /* sum of proper divisors */
        if (S >= (seq_t)tested) {
            continue;   /* not deficient */
        }

        /* Target defect to represent as subset sum of proper divisors */
        uint64_t defect = (uint64_t)((seq_t)tested - S);
        if (defect == 0) continue;              /* perfect numbers excluded */

        /* Factorise u into primes */
        uint64_t primes[64];
        uint64_t exps[64];
        size_t pf_cnt = 0;
        factor_u64(u, primes, exps, &pf_cnt);

        /* Generate all divisors of u */
        uint64_t divisors[2048];
        size_t div_cnt = 1;
        divisors[0] = 1;
        for (size_t i = 0; i < pf_cnt; ++i) {
            uint64_t p = primes[i];
            uint64_t e = exps[i];
            size_t cur = div_cnt;
            uint64_t pow = 1;
            for (uint64_t k = 1; k <= e; ++k) {
                pow *= p;
                for (size_t j = 0; j < cur; ++j) {
                    divisors[div_cnt++] = divisors[j] * pow;
                }
            }
        }

        /* Subset‑sum DP to see if defect can be formed from proper divisors */
        size_t target = (size_t)defect;
        bool *dp = (bool *)calloc(target + 1, sizeof(bool));
        if (!dp) abort();   /* out of memory */
        dp[0] = true;

        for (size_t i = 0; i < div_cnt; ++i) {
            uint64_t d = divisors[i];
            if (d == u) continue;               /* skip the number itself */
            if (d > target) continue;
            size_t sd = (size_t)d;
            for (size_t s = target; s >= sd; --s) {
                if (dp[s - sd]) dp[s] = true;
            }
        }

        if (dp[target]) {
            append_term((seq_t)u);              /* u satisfies the property */
        }

        free(dp);
    }
    return terms[n - 1];
}
