/*
 * A363647  Expansion of Sum_{k>0} (1/(1 - (k*x)^k)^3 - 1).
 * https://oeis.org/A363647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (n/d)^n * binomial(d+2,2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   binomial, factor_u64, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

static void process_divs(idx_t n, const uint64_t *pr, const uint64_t *ex,
                         size_t cnt, size_t idx, seq_t cur, seq_t *sum)
{
    if (idx == cnt) {
        seq_t nd = (seq_t)n / cur;                           /* (n/d) */
        seq_t pow_term = seq_pow(nd, n);                      /* (n/d)^n */
        seq_t bin = binomial((idx_t)((uint64_t)cur + 2), (idx_t)2); /* binomial(d+2,2) */
        seq_t term = seq_mul(pow_term, bin);                 /* product */
        *sum += term;
        return;
    }
    uint64_t p = pr[idx];
    uint64_t e = ex[idx];
    seq_t val = cur;
    for (uint64_t k = 0; k < e; ++k) {
        process_divs(n, pr, ex, cnt, idx + 1, val, sum);     /* exponent k */
        val *= (seq_t)p;                                    /* multiply for next exponent */
    }
    process_divs(n, pr, ex, cnt, idx + 1, val, sum);         /* exponent e */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0, but safe fallback */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);              /* prime factorization */
    seq_t sum = 0;
    process_divs(n, primes, exps, cnt, 0, (seq_t)1, &sum);   /* enumerate divisors */
    return sum;
}
