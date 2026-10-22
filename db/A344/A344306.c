/*
 * A344306  Number of cyclic subgroups of the group (C_n)^10, where C_n is the
 *          cyclic group of order n.
 * https://oeis.org/A344306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} [ 1 + ((p^10−1)/(p−1)) · ((p^(9e)−1)/(p^9−1)) ]
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 119,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function factors n, computes the multiplicative contribution of each
   prime power using only additions and multiplications in the seq_t type. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n <= 1)
        return (seq_t)n;            /* a(0)=0 (unused), a(1)=1 */

    /* factor n into prime powers */
    uint64_t temp = n;
    uint64_t primes[32];
    int exps[32];
    int cnt = 0;

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt]  = e;
            ++cnt;
        }
    }
    if (temp > 1) {                   /* remaining prime factor */
        primes[cnt] = temp;
        exps[cnt]    = 1;
        ++cnt;
    }

    seq_t result = 1;

    /* multiply the contribution of each prime power */
    for (int i = 0; i < cnt; ++i) {
        seq_t p_t = (seq_t)primes[i];
        int e     = exps[i];

        /* compute sum10 = 1 + p + p^2 + ... + p^9 and p^9 */
        seq_t sum10 = 1;
        seq_t term  = 1;
        seq_t p9    = 1;
        for (int k = 1; k <= 9; ++k) {
            term   *= p_t;   /* p^k */
            sum10 += term;
            if (k == 9)
                p9 = term;   /* p^9 */
        }

        /* compute sum9e = 1 + p^9 + p^(18) + ... + p^(9(e-1)) */
        seq_t sum9e   = 1;
        seq_t term_p9 = 1;
        for (int k = 1; k < e; ++k) {
            term_p9 *= p9;   /* (p^9)^k */
            sum9e   += term_p9;
        }

        /* contribution of this prime power */
        seq_t part = (seq_t)1 + sum10 * sum9e;
        result *= part;
    }

    return result;
}
