/*
 * A358434  Number of odd middle divisors of n, where "middle divisor" means a
 *          divisor in the half-open interval [sqrt(n/2), sqrt(n*2)).
 * https://oeis.org/A358434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of odd divisors of n in the half‑open interval
 *            [sqrt(n/2), sqrt(2n)). The code factors n, enumerates its odd
 *            divisors, and counts those satisfying d^2 >= ceil(n/2) and d^2
 *            <= 2n-1
 * Range:     u8 n <= 16, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Recursive helper that builds all divisors from the factorisation
   of the odd part of n. */
static void gen_divs_rec(size_t idx, seq_t curr,
                         const uint64_t *primes, const uint64_t *exps,
                         size_t num_primes,
                         seq_t *divs, size_t *divs_cnt)
{
    if (idx == num_primes) {
        divs[*divs_cnt] = curr;
        (*divs_cnt)++;
        return;
    }
    seq_t p = (seq_t)primes[idx];
    seq_t p_pow = 1;
    for (size_t k = 0; k <= exps[idx]; ++k) {
        gen_divs_rec(idx + 1, curr * p_pow,
                     primes, exps, num_primes,
                     divs, divs_cnt);
        p_pow *= p;
    }
}

/* The required function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;               /* undefined for n==0, return 0 safely */

    /* Remove all factors of 2 – only odd divisors contribute. */
    uint64_t odd = (uint64_t)n;
    while ((odd & 1) == 0) odd >>= 1;

    /* Factorise the odd part.  The helper supplies arrays of primes
       and their exponents.  Maximum number of prime factors for
       64‑bit numbers is < 64, so a static array of size 64 suffices. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(odd, primes, exps, &cnt);

    /* Generate all divisors of the odd part.  A divisor count for
       numbers up to 10⁴ is well below 256, so a small static array
       is enough. */
    seq_t divs[256];
    size_t divs_cnt = 0;
    gen_divs_rec(0, (seq_t)1, primes, exps, cnt, divs, &divs_cnt);

    /* Thresholds for the interval [sqrt(n/2), sqrt(2n)).  Using
       integer arithmetic:

           d^2 >= ceil(n/2)   ->  d^2 >= (n+1)/2
           d^2 < 2n           ->  d^2 <= 2n-1          */
    idx_t thr_low  = ((idx_t)n + 1) / 2;        /* ceil(n/2)   */
    idx_t thr_high = (idx_t)(2 * (seq_t)n - 1);   /* 2n-1        */

    seq_t count = 0;
    for (size_t i = 0; i < divs_cnt; ++i) {
        seq_t d   = divs[i];
        seq_t d2  = d * d;                     /* d^2       */
        /* Since all divisors are odd, no extra test for parity is needed. */
        if (d2 >= (seq_t)thr_low && d2 <= (seq_t)thr_high) {
            count++;
        }
    }
    return count;
}
