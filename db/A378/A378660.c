/*
 * A378660  Smallest divisor d of n for which n/d is non-abundant, i.e.
 *          sigma(n/d) <= 2*(n/d).
 * https://oeis.org/A378660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = smallest divisor d of n such that n/d is non‑abundant *
 *            (sigma(n/d) <= 2 * (n/d)). Equivalent: let m = n/d be the
 *            largest * divisor of n with sigma(m) <= 2m; then a(n) = n / m
 * Range:     u8 n <= 605, u16 n <= 9267, u32 n <= 9267, u64 n <= 9267,
 *            u128 n <= 9267, big any n
 * Helpers:   factor_u64, recurse_div
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Necessary type definitions are supplied by the build (seq_t and idx_t). */
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */
#include "../utils/recurse_div.h"

seq_t a(idx_t n)
{
    if (n < 1)                /*  Undefined for n == 0, but return 0 safely  */
        return (seq_t)0;

    /*  Prime factorisation of n  */
    uint64_t primes[64], exps[64];
    size_t   pf_cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pf_cnt);

    /*  Pre‑compute powers p^e and the corresponding sigma(p^e)
     *  sigma(p^e) = 1 + p + p^2 + ... + p^e   */
    seq_t pow_arr[64][65];
    seq_t sigma_arr[64][65];

    for (size_t i = 0; i < pf_cnt; ++i) {
        pow_arr[i][0]    = (seq_t)1;
        sigma_arr[i][0]  = (seq_t)1;          /* sigma(p^0) = 1  */
        for (uint64_t k = 1; k <= exps[i]; ++k) {
            pow_arr[i][k]   = pow_arr[i][k - 1] * (seq_t)primes[i];
            sigma_arr[i][k] = sigma_arr[i][k - 1] + pow_arr[i][k];
        }
    }

    /*  Find the largest divisor m of n that is non‑abundant.  */
    seq_t best_m = (seq_t)1;
    recurse_div(0, pf_cnt, pow_arr, sigma_arr,
                exps, (seq_t)1, (seq_t)1, &best_m);

    /*  The desired term is n divided by that largest divisor.  */
    return ((seq_t)n) / best_m;
}
