/*
 * A276668  a(n) = denominator(b(2*n)) with b(n) =
 *          n!*Bernoulli(n,1/2)/(n*(n-1)) for n>=2, b(0)=1, b(1)=0.
 * https://oeis.org/A276668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   _v2_factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes the OEIS sequence A276668,
   which is defined as a(n) = denominator(b(2*n)),
   where for n≥2:
          b(n) = n! * Bernoulli(n, 1/2) / (n*(n-1)),
   with b(0)=1 and b(1)=0.
   Using the identity
          B_n(1/2) = (1-2^{1-n}) * B_n,
   we obtain
          b(n) = (n-2)! * (2^{1-n}-1) * B_n.
   The denominator of b(2n) is thus
          D = 2^{2n-1} * D_B / gcd(2^{2n-1}*D_B, (2n-2)! * (1-2^{2n-1})),
   where D_B is the denominator of the ordinary Bernoulli number B_{2n}
   (given by the von Staudt–Clausen theorem: D_B = ∏_{p-1 | 2n} p).
   Since B_{2n} has numerator coprime with D_B, only the factor (2n-2)! can
   cancel primes from D_B.  This leads to the concise algorithm:
       let e2 = 2*n – v_2((2n-2)!)   // exponent of 2 remaining after cancellation
       result = 2^e2
       for each odd prime p|D_B with p > 2n-2
           result *= p
   The function below implements this.
   It lazily precomputes primes up to the largest 2*n encountered.

   The computation uses only integer arithmetic on the type seq_t,
   which may be an arbitrary precision integer when compiled as C++17.  */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/_v2_factorial.h"

static size_t _primes_cnt = 0;
static uint64_t *_primes = NULL;
static idx_t _primes_limit = 0;

/* Ensure that the array _primes contains all primes ≤ limit. */
static void _ensure_primes_upto(idx_t limit)
{
    if (limit <= _primes_limit) return;

    /* free old buffer if any */
    free(_primes);
    _primes = NULL;
    _primes_cnt = 0;

    /* simple sieve up to limit */
    uint8_t *is_pr = (uint8_t *)malloc((size_t)(limit + 1));
    if (!is_pr) return;                           /* out of memory → just skip */
    for (idx_t i = 0; i <= limit; ++i) is_pr[i] = 1;
    is_pr[0] = is_pr[1] = 0;
    for (idx_t i = 2; i * i <= limit; ++i)
        if (is_pr[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_pr[j] = 0;

    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_pr[i]) ++cnt;

    _primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    size_t k = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_pr[i]) _primes[k++] = (uint64_t)i;

    free(is_pr);
    _primes_cnt = cnt;
    _primes_limit = limit;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* b(0)=1 → denominator 1 */

    /* two_n = 2*n */
    idx_t two_n = n + n;
    /* factorial exponent (2n-2)! */
    idx_t m = two_n - 2;                  /* safe: for n≥1, two_n≥2 */
    idx_t exp2_fact = _v2_factorial(m);
    idx_t exp2_final = two_n - exp2_fact; /* exponent of 2 remaining */

    /* start with 2^exp2_final */
    seq_t result = (seq_t)1;
    if (exp2_final > 0)
        result <<= exp2_final;

    /* primes needed for D_B */
    idx_t limit = two_n + 1;
    _ensure_primes_upto(limit);

    /* multiply by odd primes p where p-1 | 2n and p > 2n-2 */
    for (size_t i = 0; i < _primes_cnt; ++i) {
        uint64_t p = _primes[i];
        if (p == 2) continue;                     /* 2 handled by exponent */
        if (two_n % (p - 1) != 0) continue;       /* not in D_B */
        if (p <= two_n - 2) continue;             /* cancels in factorial */
        result *= (seq_t)p;
    }

    return result;
}
