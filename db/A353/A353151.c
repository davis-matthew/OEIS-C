/*
 * A353151  A Gaussian integer analog of the sum-of-divisors function (see
 *          Comments lines for definition).
 * https://oeis.org/A353151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) is the product over the Gaussian prime factorization of n.
 *            For each ordinary prime factor p^e of n: – If p ≡ 3 (mod 4), use
 *            the Gaussian prime p (p stays prime in ℤ[i]), contribution =
 *            (p^(e+1) – 1)/(p – 1). – If p = 2, use q = 1 + i (the unique
 *            Gaussian prime with norm 2), contribution = |∑...
 * Range:     u8 n <= 18, u16 n <= 4418, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/floor_sqrt.h"
#include "../utils/big_pow.h"

#if defined(_MSC_VER)
    #define NO_INLINE __forceinline
#else
    #define NO_INLINE inline
#endif

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* factorize n over ordinary integers */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* p ≡ 3 (mod 4) – remains Gaussian prime */
        if (p % 4 == 3) {
            seq_t pw = big_pow((seq_t)p, (seq_t)(e + 1));
            seq_t part = (pw - (seq_t)1) / ((seq_t)p - (seq_t)1);
            result *= part;
            continue;
        }

        /* p == 2 or p ≡ 1 (mod 4) – splits into conjugate pair */
        seq_t a_seq, b_seq;

        if (p == 2) {           /* unique associate 1+i */
            a_seq = (seq_t)1;
            b_seq = (seq_t)1;
        } else {                /* find integers a,b with a²+b²=p */
            uint64_t sqrtp = (uint64_t)floor_sqrt((seq_t)p);
            for (uint64_t a = sqrtp; a < sqrtp + 1; --a) {
                seq_t a_val = (seq_t)a;
                seq_t aa = a_val * a_val;
                seq_t p_val = (seq_t)p;
                if (aa > p_val) continue;          /* safety */
                seq_t t = p_val - aa;
                seq_t b_val = floor_sqrt(t);
                seq_t bb = b_val * b_val;
                if (bb == t) {                     /* representation found */
                    a_seq = a_val;
                    b_seq = b_val;
                    break;
                }
                if (a == 0) break;                 /* should never happen */
            }
        }

        /* compute S = ∑_{k=0}^e q^k,  q = a + bi */
        seq_t sum_r = (seq_t)1, sum_i = (seq_t)0;
        seq_t term_r = (seq_t)1, term_i = (seq_t)0;

        for (uint64_t k = 1; k <= e; ++k) {
            seq_t nr = term_r * a_seq - term_i * b_seq;
            seq_t ni = term_r * b_seq + term_i * a_seq;
            term_r = nr;
            term_i = ni;
            sum_r += term_r;
            sum_i += term_i;
        }

        seq_t part = sum_r * sum_r + sum_i * sum_i;   /* |S|² */
        result *= part;
    }

    return result;
}
