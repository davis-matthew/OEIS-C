/*
 * A264390  Partial sums of A267326.
 * https://oeis.org/A264390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} A000118(k^2) A000118(m) = 8 * Σ_{d|m, 4∤d} d
 *            (ordered 4‑square representations) For k^2 this simplifies to:
 *            if k is odd: A000118(k^2) = 8 * σ(k^2) if k is even: k = 2^e * m
 *            (m odd) -> A000118(k^2) = 24 * σ(m^2) where σ(x) is the sum of
 *            divisors of x. The function caches partia...
 * Range:     u8 n <= 4, u16 n <= 29, u32 n <= 150, u64 n <= 150,
 *            u128 n <= 150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static int   spf[10001];                /* smallest prime factor */
static uint32_t primes[1229];           /* π(10000) ≈ 1229 */
static size_t  prime_cnt = 0;
static size_t  sieve_limit = 0;

/* initialise sieve up to at least 'limit' */
static void
init_sieve(size_t limit)
{
    if (limit <= sieve_limit)
        return;
    sieve_limit = limit;
    for (size_t i = 2; i <= limit; ++i)
        spf[i] = 0;
    prime_cnt = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (int)i;
            primes[prime_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < prime_cnt &&
                          primes[j] <= (uint32_t)spf[i] &&
                          i * (uint64_t)primes[j] <= limit; ++j) {
            spf[i * (size_t)primes[j]] = (int)primes[j];
        }
    }
}

/* fast exponentiation for seq_t */
static seq_t
pow_uint(uint64_t base, uint32_t exp)
{
    seq_t result = 1;
    seq_t b = (seq_t)base;
    while (exp) {
        if (exp & 1U)
            result *= b;
        b *= b;
        exp >>= 1U;
    }
    return result;
}

/* sum of divisors of n^2; n is assumed >0 and <= sieve_limit */
static seq_t
sigma_square_of_number(uint32_t n)
{
    seq_t sigma = 1;
    uint32_t x = n;
    while (x > 1) {
        int p = spf[x];
        uint32_t e = 0;
        while (x % p == 0) {
            x /= p;
            ++e;
        }
        /* term = (p^(2e+1) - 1) / (p - 1) */
        seq_t num = pow_uint((uint64_t)p, 2U * e + 1U) - (seq_t)1;
        seq_t term = num / (p - 1U);
        sigma *= term;
    }
    return sigma;
}

/* cache of partial sums a(k) */
static seq_t *cache = NULL;
static size_t cache_size = 0;

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* below offset, value arbitrary */
    init_sieve(10000);
    /* allocate cache if necessary */
    if (cache == NULL) {
        cache = (seq_t *)malloc((10001) * sizeof(seq_t));
        if (!cache) return (seq_t)0;
        cache[0] = 0;
        cache_size = 1;
    }
    /* extend cache up to n */
    if ((size_t)n + 1 > cache_size) {
        size_t old = cache_size;
        cache_size = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;
        /* compute values from old to n */
        for (idx_t k = (idx_t)old; k <= n; ++k) {
            uint32_t kk = (uint32_t)k;
            uint32_t exp2 = 0;
            while ((kk & 1U) == 0U) {
                exp2++;
                kk >>= 1U;
            }
            seq_t r4;
            if (exp2 == 0) {                     /* k is odd */
                seq_t sigma = sigma_square_of_number((uint32_t)k);
                r4 = (seq_t)8 * sigma;
            } else {                              /* k even */
                uint32_t m = (uint32_t)k >> exp2; /* odd part */
                seq_t sigma = sigma_square_of_number(m);
                r4 = (seq_t)24 * sigma;
            }
            cache[k] = cache[k - 1] + r4;
        }
    }
    return cache[n];
}
