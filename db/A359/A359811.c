/*
 * A359811  a(n) = Sum_{d|n} 2^(d-1) * d^(n/d-1).
 * https://oeis.org/A359811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} 2^(d-1) * d^(n/d-1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

/* ------------------------------------------------------------------ */
/* Recursively generate all divisors from the prime factorization.   */
/* ------------------------------------------------------------------ */
static void gen_divisors(const uint64_t *pr, const uint64_t *ex, size_t cnt,
                         size_t pos, uint64_t cur, uint64_t *out, size_t *outcnt)
{
    if (pos == cnt) {
        out[(*outcnt)++] = cur;
        return;
    }
    uint64_t v = 1;
    for (size_t e = 0; e <= ex[pos]; ++e) {
        gen_divisors(pr, ex, cnt, pos + 1, cur * v, out, outcnt);
        if (e < ex[pos]) v *= pr[pos];
    }
}

/* ------------------------------------------------------------------ */
/* Main function computing a(n).  Any n >= 1.                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Factor n.  For values up to 10 000 the number of distinct primes is
       small, so a fixed small table suffices. */
    uint64_t nn   = (uint64_t)n;
    uint64_t primes[16];
    uint64_t exps[16];
    size_t pcnt = 0;
    factor_u64(nn, primes, exps, &pcnt);

    /* Estimate divisor count (safe upper bound). */
    size_t divcnt = 1;
    for (size_t i = 0; i < pcnt; ++i) divcnt *= (exps[i] + 1);
    if (divcnt > 2048) divcnt = 2048;                          /* safety */

    uint64_t divs[2048];
    size_t divc = 0;
    gen_divisors(primes, exps, pcnt, 0, 1, divs, &divc);

    /* Sum of 2^(d-1) * d^(n/d-1) over all divisors d of n. */
    seq_t sum = 0;
    for (size_t i = 0; i < divc; ++i) {
        uint64_t d      = divs[i];
        idx_t   exp_d   = (idx_t)d - 1;                       /* d-1   */
        idx_t   exp_q   = n / d - 1;                          /* n/d-1 */
        seq_t  term1 = big_pow((seq_t)2,  (seq_t)exp_d);
        seq_t  term2 = big_pow((seq_t)d,  (seq_t)exp_q);
        sum += term1 * term2;
    }
    return sum;
}
