/*
 * A378662  Number of divisors d of n such that sigma(d) <= 2*d < A003961(d),
 *          where A003961 is fully multiplicative with a(p) = nextprime(p).
 * https://oeis.org/A378662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of n such that sigma(d) <= 2*d <
 *            ∏_{p|d} nextprime(p)^{v_p(d)} (A003961)
 * Range:     u8 n <= 62, u16 n <= 8816, u32 n <= 8816, u64 n <= 8816,
 *            u128 n <= 8816, big any n
 * Helpers:   factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"

/* Recursive helper that enumerates all divisors of n */
static void dfs(uint64_t i,
                uint64_t num,
                const uint64_t *pr,
                const uint64_t *ex,
                seq_t cur_d,
                seq_t sigma_cur,
                seq_t np_prod,
                seq_t *cnt)
{
    if (i == num) {
        seq_t two_d = ((seq_t)2) * cur_d;
        if (sigma_cur <= two_d && two_d < np_prod)
            (*cnt)++;
        return;
    }

    uint64_t p = pr[i];
    uint64_t e_max = ex[i];
    uint64_t np = next_prime(p);

    seq_t p_pow = 1;           /* p^e                        */
    seq_t sigma_pe = 1;        /* σ(p^e)                      */
    seq_t np_pe = 1;           /* nextprime(p)^e              */

    for (uint64_t e = 0; e <= e_max; ++e) {
        /* For e == 0 the values are already correct (1) */
        /* For e > 0 update the products */
        if (e > 0) {
            p_pow = p_pow * ((seq_t)p);
            sigma_pe = sigma_pe + p_pow;       /* add p^e to σ(p^e) */
            np_pe = np_pe * ((seq_t)np);       /* multiply by nextprime(p) */
        }

        seq_t new_d = cur_d * p_pow;
        seq_t new_sigma = sigma_cur * sigma_pe;
        seq_t new_np = np_prod * np_pe;

        dfs(i + 1, num, pr, ex, new_d, new_sigma, new_np, cnt);
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t num = 0;
    factor_u64((uint64_t)n, primes, exps, &num);

    seq_t result = 0;
    dfs(0, num, primes, exps, (seq_t)1, (seq_t)1, (seq_t)1, &result);
    return result;
}
