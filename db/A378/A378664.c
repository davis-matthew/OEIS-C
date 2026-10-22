/*
 * A378664  Greatest divisor d of n such that sigma(d) <= 2*d < A003961(d), or
 *          1 if no such divisor exists, where A003961 is fully multiplicative
 *          with a(p) = nextprime(p).
 * https://oeis.org/A378664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Greatest divisor d of n such that sigma(d) <= 2*d < A003961(d),
 *            or 1 if no such divisor exists
 * Range:     u8 n <= 62, u16 n <= 9995, u32 n <= 9995, u64 n <= 9995,
 *            u128 n <= 9995, big any n
 * Helpers:   _smallest_prime_factor, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include helper headers */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/_smallest_prime_factor.h"
#include "../utils/next_prime.h"

/* Recursive helper to enumerate all divisors of n. */
static void rec_helper(int idx, seq_t cur_d, seq_t *best_d,
                       const uint64_t *primes, const int *exps, int pcount,
                       int *chosen_exp)
{
    if (idx == pcount) {
        /* Compute sigma(d) and A003961(d) for this divisor. */
        seq_t sigma   = 1;
        seq_t A003961 = 1;

        for (int j = 0; j < pcount; ++j) {
            uint64_t p = primes[j];
            int e = chosen_exp[j];

            /* sigma term: sum_{i=0}^{e} p^i */
            seq_t term_sigma = 1;
            seq_t pow = 1;
            for (int k = 1; k <= e; ++k) {
                pow *= (seq_t)p;
                term_sigma += pow;
            }
            sigma *= term_sigma;

            /* A003961 term: nextprime(p)^e */
            uint64_t nxt = next_prime(p);
            seq_t term_a = 1;
            for (int k = 0; k < e; ++k) {
                term_a *= (seq_t)nxt;
            }
            A003961 *= term_a;
        }

        seq_t two_d = cur_d + cur_d;
        if (sigma <= two_d && two_d < A003961) {
            if (cur_d > *best_d)
                *best_d = cur_d;
        }
        return;
    }

    uint64_t p = primes[idx];
    int maxe = exps[idx];
    seq_t factor = 1;

    for (int e = 0; e <= maxe; ++e) {
        chosen_exp[idx] = e;
        rec_helper(idx + 1, cur_d * factor, best_d,
                   primes, exps, pcount, chosen_exp);
        factor *= (seq_t)p;   /* increment factor to p^(e+1) */
    }
}

seq_t a(idx_t n)
{
    /* Factor n into primes and exponents. */
    uint64_t temp = (uint64_t)n;
    uint64_t primes[64];
    int exps[64];
    int pcount = 0;

    if (temp > 1) {
        while (temp > 1) {
            uint64_t p = _smallest_prime_factor(temp);
            int cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
            }
            primes[pcount] = p;
            exps[pcount]   = cnt;
            ++pcount;
        }
    }

    seq_t best_d = 1;
    int chosen_exp[64] = {0};

    rec_helper(0, 1, &best_d, primes, exps, pcount, chosen_exp);

    return best_d;
}
