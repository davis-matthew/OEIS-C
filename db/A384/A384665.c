/*
 * A384665  Smallest odd multiplier k such that k*n is abundant.
 * https://oeis.org/A384665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest odd multiplier k such that k*n is abundant (k*n
 *            has sigma(k*n) > 2*k*n)
 * Range:     u8 none, u16 n <= 1582, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Pre‑computed prime factorisation of all odd k <= 945 */
static uint64_t k_primes[946][10];
static int      k_exps   [946][10];
static size_t   k_cnt    [946];

static bool k_prepared = false;

static seq_t sigma_from_factors(const uint64_t *pr, const int *ex, size_t cnt) {
    seq_t sigma = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term  = 1;
        seq_t power = 1;
        seq_t p_seq = (seq_t)pr[i];
        int   e     = ex[i];
        for (int j = 0; j < e; ++j) {
            power = power * p_seq;
            term  = term + power;
        }
        sigma = sigma * term;
    }
    return sigma;
}

seq_t a(idx_t n) {
    /* prepare factorisation table for odd k once */
    if (!k_prepared) {
        for (idx_t k = 1; k <= 945; k += 2) {
            uint64_t pr[64];
            uint64_t ex[64];
            size_t   cnt;
            factor_u64((uint64_t)k, pr, ex, &cnt);
            k_cnt[k] = cnt;
            for (size_t i = 0; i < cnt; ++i) {
                k_primes[k][i] = pr[i];
                k_exps   [k][i] = (int)ex[i];
            }
        }
        k_prepared = true;
    }

    /* factorise n once */
    uint64_t n_pr[64];
    uint64_t n_ex[64];
    size_t   n_cnt;
    factor_u64((uint64_t)n, n_pr, n_ex, &n_cnt);

    /* try each odd k until abundant is reached */
    for (idx_t k = 1; k <= 945; k += 2) {
        /* combine prime factors of n and k */
        size_t tcnt = n_cnt;
        uint64_t tpr[64];
        int       tex[64];
        for (size_t i = 0; i < tcnt; ++i) {
            tpr[i] = n_pr[i];
            tex[i] = (int)n_ex[i];
        }

        size_t kc = k_cnt[k];
        for (size_t i = 0; i < kc; ++i) {
            uint64_t pk = k_primes[k][i];
            int      ek = k_exps   [k][i];
            bool found = false;
            for (size_t j = 0; j < tcnt; ++j) {
                if (tpr[j] == pk) {
                    tex[j] += ek;
                    found = true;
                    break;
                }
            }
            if (!found) {
                tpr[tcnt] = pk;
                tex[tcnt] = ek;
                ++tcnt;
            }
        }

        /* compute sigma of the merged factors */
        seq_t sigma = sigma_from_factors(tpr, tex, tcnt);

        seq_t m        = ((seq_t)n) * (seq_t)k;
        seq_t threshold = m + m;
        if (sigma > threshold) {
            return k;
        }
    }
    /* The sequence guarantees a solution <= 945,
       but in practice we always return inside the loop. */
    return 945;
}
