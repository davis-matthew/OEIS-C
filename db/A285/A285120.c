/*
 * A285120  Min(|d(k+1-i) - d(i)|, for i = 1..k), where d(1),..,d(k) are the
 *          divisors of n(n+1)/2.
 * https://oeis.org/A285120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = min{|d(k+1-i) - d(i)|} for i=1..k, where d(1)..d(k) are
 *            the divisors of n(n+1)/2. This is equivalent to a(n) =
 *            T/best_div - best_div, where T = n(n+1)/2 and best_div is the
 *            largest divisor of T not exceeding sqrt(T)
 * Range:     u8 n <= 10, u16 n <= 72, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, floor_sqrt, gen_max_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* utility includes */
#include "../utils/floor_sqrt.h"
#include "../utils/build_primes_upto.h"
#include "../utils/gen_max_divisor.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* T = n(n+1)/2 */
    seq_t T = (seq_t)n * ((seq_t)n + (seq_t)1) / (seq_t)2;

    /* handle trivial case n = 1 */
    if (n == 1)
        return 0;

    /* sqrtT as seq_t and uint64_t (fits for n up to ~10^4) */
    seq_t sqrtT_seq = floor_sqrt(T);
    uint64_t sqrtT = (uint64_t)sqrtT_seq;

    /* generate primes up to sqrtT */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(sqrtT, &primes, &prime_cnt);

    /* factorise T (uint64_t is sufficient for tested ranges) */
    uint64_t T64 = (uint64_t)T;
    uint64_t rem = T64;

    uint64_t pf_pri[64];
    int       pf_exp[64];
    int pf_cnt = 0;

    for (size_t i = 0; i < prime_cnt && rem > 1; ++i) {
        uint64_t p = primes[i];
        if (p * p > rem)
            break;
        if (rem % p == 0) {
            int e = 0;
            while (rem % p == 0) {
                rem /= p;
                ++e;
            }
            pf_pri[pf_cnt] = p;
            pf_exp[pf_cnt] = e;
            ++pf_cnt;
        }
    }
    if (rem > 1) {
        pf_pri[pf_cnt] = rem;
        pf_exp[pf_cnt] = 1;
        ++pf_cnt;
    }

    /* find the largest divisor ≤ sqrtT */
    seq_t best_div = (seq_t)1;
    gen_max_divisor(0, (seq_t)1, pf_pri, pf_exp,
                    pf_cnt, sqrtT_seq, &best_div);

    /* clean up */
    free(primes);

    /* a(n) = T / best_div - best_div */
    return T / best_div - best_div;
}
