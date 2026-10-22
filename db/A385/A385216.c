/*
 * A385216  Greatest Heinz number of a sparse submultiset of the prime indices
 *          of n, where a multiset is sparse iff 1 is not a first difference.
 * https://oeis.org/A385216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   build_primes_upto, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = maximum Heinz number of a sparse submultiset of the prime indices of n
    (equivalently, maximum product of primes p^e taken from the prime factorisation
    of n such that no two chosen indices differ by 1).  */

#include <stdlib.h>
#include <stdbool.h>

/* Utilities required from the build */
#include "../utils/pow_uint.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                    /* offset starts at 1; undefined otherwise */

    /* 1. Build list of all primes up to n (enough for factorisation and index lookup). */
    uint64_t *prime_list = NULL;
    size_t prime_cnt   = 0;
    build_primes_upto(n, &prime_list, &prime_cnt);
    if (!prime_list || prime_cnt == 0) {
        /* fallback for n = 0 or 1 */
        return (seq_t)1;
    }

    /* 2. Factor n.  Store distinct prime factors and their exponents. */
    uint64_t factors[prime_cnt];
    uint32_t exps[prime_cnt];
    size_t nfactors = 0;
    uint64_t rem = n;
    for (size_t i = 0; i < prime_cnt && rem > 1; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > rem)
            break;
        uint32_t cnt = 0;
        while (rem % p == 0) {
            rem /= p;
            ++cnt;
        }
        if (cnt) {
            factors[nfactors] = p;
            exps[nfactors]    = cnt;
            ++nfactors;
        }
    }
    if (rem > 1) {                     /* remaining prime factor > sqrt(n) */
        factors[nfactors] = rem;
        exps[nfactors]    = 1;
        ++nfactors;
    }

    /* 3. For each factor, obtain its index in the prime sequence and the weight p^e. */
    idx_t   idxs[nfactors];
    seq_t   weights[nfactors];
    for (size_t i = 0; i < nfactors; ++i) {
        uint64_t p = factors[i];

        /* Find index by linear search (prime_cnt is < 1300 for n ≤ 10000). */
        idx_t idx = 0;
        for (size_t j = 0; j < prime_cnt; ++j) {
            if (prime_list[j] == p) {
                idx = (idx_t)(j + 1);   /* 1‑based index */
                break;
            }
        }

        idxs[i]    = idx;
        weights[i] = pow_uint(p, exps[i]);  /* p^e */
    }

    /* 4. DP over the indices (sorted ascending automatically). */
    seq_t best0 = 1;            /* best product when previous index not chosen */
    seq_t best1 = 0;            /* best product when previous index chosen */
    idx_t prev_idx = 0;         /* no previous index yet */

    for (size_t i = 0; i < nfactors; ++i) {
        idx_t cur_idx = idxs[i];
        seq_t w = weights[i];
        seq_t new_best1;
        seq_t new_best0;

        if (cur_idx == prev_idx + 1) {            /* consecutive index – cannot pick both */
            new_best1 = best0 * w;
        } else {                                  /* no adjacency conflict */
            seq_t max_all = (best0 > best1 ? best0 : best1);
            new_best1 = max_all * w;
        }

        new_best0 = (best0 > best1 ? best0 : best1);

        best0 = new_best0;
        best1 = new_best1;
        prev_idx = cur_idx;
    }

    seq_t result = (best0 > best1 ? best0 : best1);

    /* 5. Clean up */
    free(prime_list);

    return result;
}
