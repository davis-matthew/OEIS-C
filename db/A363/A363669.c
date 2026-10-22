/*
 * A363669  a(n) = Sum_{d|n} (n/d)^n * binomial(d+n-1,d).
 * https://oeis.org/A363669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (n/d)^n * binomial(d+n-1,d)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow, binomial, factor_u64, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"
#include "../utils/factor_u64.h"
#include "../utils/seq_mul.h"

/* Enumerate all divisors of n from its prime factorization.
   For each divisor d, accumulate the term (n/d)^n * binomial(d+n-1,d) into *sum. */
static void
add_divisor_terms(const uint64_t *pr, const uint64_t *ex, size_t idx, uint64_t cur,
                  seq_t *sum, idx_t n,
                  size_t cnt)
{
    if (idx == cnt) {
        /* cur is a divisor d of n */
        seq_t base = (seq_t)(n / cur);          /* n/d */
        seq_t pow  = big_pow(base, (seq_t)n);   /* (n/d)^n */
        seq_t bin  = binomial(cur + n - 1, cur);
        seq_t term = seq_mul(pow, bin);
        *sum = (*sum) + term;                   /* accumulate */
        return;
    }
    /* Try exponents 0 .. ex[idx] for prime pr[idx] */
    uint64_t d = cur;
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        add_divisor_terms(pr, ex, idx + 1, d, sum, n, cnt);
        d *= pr[idx];
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* undefined for n=0, but return 0 */

    seq_t sum = 0;
    /* Factor n into primes */
    uint64_t prime[64];
    uint64_t exp[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, prime, exp, &cnt);

    add_divisor_terms(prime, exp, 0, 1, &sum, n, cnt);
    return sum;
}
