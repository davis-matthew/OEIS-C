/*
 * A339872  Index k of the least prime(k) such that prime(k)-adic valuation of
 *          phi(n) exceeds the prime(k)-adic valuation of n-1, or 0 if no such
 *          k exists (for example, when n = 1 or a prime).
 * https://oeis.org/A339872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A055396(A160595(n))
 * Range:     u8 n <= 1728, u16 n <= 8788, u32 n <= 8788, u64 n <= 8788,
 *            u128 n <= 8788, big any n
 * Helpers:   build_primes_upto, calc_phi, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the index k of the least prime p_k such that v_{p_k}(phi(n)) > v_{p_k}(n‑1),
   or 0 if no such k exists. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"
#include "../utils/calc_phi.h"
#include "../utils/build_primes_upto.h"

/* Pre‑computed primes up to a fixed limit */
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;

/* --------------------------------------------------------------- */
/* Ensure the prime list is built once.                           */
/* --------------------------------------------------------------- */
static void init_prime_list(void)
{
    if (prime_list == NULL) {
        /* enough to cover primes needed for n up to ~10000 */
        const idx_t limit = 200000;
        build_primes_upto(limit, &prime_list, &prime_count);
    }
}

/* --------------------------------------------------------------- */
/* Binary search for the 1‑based index of a prime p in prime_list. */
/* Returns -1 if p is not found (should not happen for primes of phi). */
/* --------------------------------------------------------------- */
static int prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (prime_list[mid] < p) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    if (lo < prime_count && prime_list[lo] == p) {
        return (int)(lo + 1);     /* 1‑based index */
    }
    return -1;
}

/* --------------------------------------------------------------- */
/* Main function.                                                   */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n <= 1) {
        return (seq_t)0;
    }

    init_prime_list();

    /* Factorization buffers */
    const size_t MAX_FACTORS = 64;
    uint64_t phi_primes[MAX_FACTORS];
    uint64_t phi_exps[MAX_FACTORS];
    size_t phi_cnt = 0;

    uint64_t n1_primes[MAX_FACTORS];
    uint64_t n1_exps[MAX_FACTORS];
    size_t n1_cnt = 0;

    /* Compute phi(n) (fits in 64 bits for n <= 10000) */
    seq_t phi_seq = calc_phi(n);
    uint64_t phi_val = (uint64_t)phi_seq;

    uint64_t n_minus_1 = (uint64_t)n - 1;

    /* Factor phi(n) and n-1 using the helper */
    factor_u64(phi_val, phi_primes, phi_exps, &phi_cnt);
    factor_u64(n_minus_1, n1_primes, n1_exps, &n1_cnt);

    /* Search for the minimal prime index satisfying the condition */
    uint64_t best_k = (uint64_t)(-1);
    for (size_t i = 0; i < phi_cnt; ++i) {
        uint64_t p = phi_primes[i];
        uint64_t e_phi = phi_exps[i];

        /* Find exponent of p in (n-1) */
        uint64_t e_n1 = 0;
        for (size_t j = 0; j < n1_cnt; ++j) {
            if (n1_primes[j] == p) {
                e_n1 = n1_exps[j];
                break;
            }
        }

        if (e_phi > e_n1) {
            int idx = prime_index(p);
            if (idx > 0) {
                if (best_k == (uint64_t)(-1) || (uint64_t)idx < best_k) {
                    best_k = (uint64_t)idx;
                    if (best_k == 1) {       /* cannot improve further */
                        break;
                    }
                }
            }
        }
    }

    if (best_k == (uint64_t)(-1)) {
        return (seq_t)0;
    } else {
        return (seq_t)best_k;
    }
}
