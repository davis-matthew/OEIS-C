/*
 * A327832  The practical component of n: the largest divisor of n which is a
 *          practical number (A005153).
 * https://oeis.org/A327832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest divisor of n that is a practical number (OEIS
 *            A327832). The algorithm factorises n, iteratively builds the
 *            product of the smallest prime factors and stops when the next
 *            prime would violate the practical-number condition: p_{j+1} >
 *            sigma(n_j) + 1. Complexity is O( sqrt(n) ) for fact...
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   power_seq_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/power_seq_u64.h"

extern uint64_t _smallest_prime_factor(uint64_t n); /* provided helper, not used below */

seq_t a(idx_t n)
{
    /* 1‑based offset */
    if (n == 0)
        return 0;         /* behaviour below offset is irrelevant */

    /* n odd ⇒ a(n) = 1 as all practical numbers >1 are even */
    if ((uint64_t)n & 1ULL)
        return 1;

    /* factor n, storing primes and exponents in small fixed arrays */
    uint64_t primes[64];
    int exps[64];
    int cnt = 0;
    uint64_t tmp = (uint64_t)n;

    int c = 0;
    while ((tmp & 1ULL) == 0ULL) {  /* factor 2 */
        ++c;
        tmp >>= 1;
    }
    if (c) {
        primes[cnt] = 2;
        exps[cnt++] = c;
    }

    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            c = 0;
            while (tmp % p == 0) {
                ++c;
                tmp /= p;
            }
            primes[cnt] = p;
            exps[cnt++] = c;
        }
    }
    if (tmp > 1) {      /* remaining prime factor */
        primes[cnt] = tmp;
        exps[cnt] = 1;
        ++cnt;
    }

    seq_t cur = 1;          /* current product n_j  */
    seq_t sigma_cur = 1;    /* sigma(cur)   */

    for (int i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        int e = exps[i];

        seq_t p_pow = power_seq_u64(p, e);        /* p^e   */
        seq_t new_cur = cur * p_pow;              /* n_j   */

        /* update sigma */
        seq_t numerator = p_pow * (seq_t)p - 1;   /* p^(e+1) - 1   */
        seq_t denom     = (seq_t)p - 1;           /* p - 1          */
        seq_t new_sigma = sigma_cur * numerator / denom;  /* sigma(new_cur) */

        /* decide whether to stop */
        if (i + 1 < cnt) {
            uint64_t next_p = primes[i + 1];
            if ((seq_t)next_p > new_sigma + 1)
                return new_cur; /* a(n) found */
        } else {
            return new_cur;     /* all primes used */
        }

        /* continue to next prime factor */
        cur = new_cur;
        sigma_cur = new_sigma;
    }

    return cur;   /* fallback – should never reach here for n>1 */
}
