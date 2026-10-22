/*
 * A327641  Number of partitions of n into divisors d of n such that n/d is
 *          squarefree.
 * https://oeis.org/A327641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into the divisors d of n such
 *            that n/d is squarefree. The algorithm finds the minimal factor M
 *            = ∏_{p^e‖n, e≥2} p^(e-1), reduces n to n' = n/M, and counts
 *            partitions of n' into its divisors by a standard
 *            dynamic‑programming routine
 * Range:     u8 n <= 29, u16 n <= 173, u32 n <= 329, u64 n <= 2309,
 *            u128 n <= 9230, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t pow_seq_uint(uint64_t base, int exp) {
    seq_t r = 1;
    for (int i = 0; i < exp; ++i) r *= base;
    return r;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;

    /* factor n */
    idx_t t = n;
    idx_t primes[16];
    int exp[16];
    int pc = 0;
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            int e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            primes[pc] = (idx_t)p;
            exp[pc] = e;
            ++pc;
        }
    }
    if (t > 1) {
        primes[pc] = t;
        exp[pc] = 1;
        ++pc;
    }

    /* compute M = product of p^(e-1) for all exponents e≥2 */
    seq_t M = 1;
    for (int i = 0; i < pc; ++i) {
        if (exp[i] >= 2) {
            M *= pow_seq_uint(primes[i], exp[i] - 1);
        }
    }

    seq_t n_prime_seq = ((seq_t)n) / M;
    idx_t np = (idx_t)n_prime_seq;

    /* collect divisors of np */
    idx_t *divs = (idx_t*)calloc(np + 1, sizeof(idx_t));
    size_t dcnt = 0;
    for (idx_t i = 1; i <= np; ++i) {
        if (np % i == 0) {
            divs[dcnt++] = i;
        }
    }

    /* DP: count partitions of np using its divisors */
    seq_t *dp = (seq_t*)calloc(np + 1, sizeof(seq_t));
    dp[0] = 1;
    for (size_t di = 0; di < dcnt; ++di) {
        idx_t d = divs[di];
        for (idx_t s = d; s <= np; ++s) {
            dp[s] += dp[s - d];
        }
    }

    seq_t result = dp[np];
    free(dp);
    free(divs);
    return result;
}
