/*
 * A341655  a(n) is the number of divisors of prime(n)^2 - 1.
 * https://oeis.org/A341655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors of prime(n)^2 - 1
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ------------------------------------------------------------
   Prime sieve (linear) and smallest prime factor array
   ------------------------------------------------------------ */
static uint32_t *primes   = NULL;   /* array of primes */
static size_t    prime_count   = 0;
static uint32_t *spf       = NULL;   /* smallest prime factor for [0..LIMIT] */
static bool      primes_ready = false;

static void init_primes_and_spf(void)
{
    if (primes_ready) return;
    /* Enough to cover the 10,000th prime (≈104,729) */
    const idx_t LIMIT = 200000;

    spf = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
    uint32_t *tmp_primes = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
    if (!spf || !tmp_primes) return;            /* allocation failed */

    for (idx_t i = 0; i <= LIMIT; ++i) spf[i] = 0;
    prime_count = 0;

    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {                     /* i is prime */
            spf[i] = (uint32_t)i;
            tmp_primes[prime_count++] = (uint32_t)i;
        }
        for (size_t j = 0; j < prime_count && tmp_primes[j] <= spf[i] && (idx_t)i * (idx_t)tmp_primes[j] <= LIMIT; ++j) {
            spf[i * tmp_primes[j]] = tmp_primes[j];
        }
    }

    primes = tmp_primes;
    primes_ready = true;
}

/* ------------------------------------------------------------
   Small factorisation using the previously built spf table
   ------------------------------------------------------------ */
static void factor_small(uint32_t x, uint32_t *factors, unsigned *exps, size_t *cnt)
{
    *cnt = 0;
    while (x > 1) {
        uint32_t pf = spf[x];
        unsigned c  = 0;
        while (x % pf == 0) { x /= pf; ++c; }
        factors[(*cnt)] = pf;
        exps[(*cnt)]    = c;
        ++(*cnt);
    }
}

/* ------------------------------------------------------------
   Merge two distinct‑prime lists into one, summing exponents
   ------------------------------------------------------------ */
static size_t merge_factors(const uint32_t *f1, const unsigned *e1, size_t cnt1,
                            const uint32_t *f2, const unsigned *e2, size_t cnt2,
                            uint32_t *fout, unsigned *eout, size_t *outcnt)
{
    size_t out = 0;
    for (size_t i = 0; i < cnt1; ++i) { fout[out] = f1[i]; eout[out] = e1[i]; ++out; }

    for (size_t i = 0; i < cnt2; ++i) {
        bool found = false;
        for (size_t j = 0; j < out; ++j) {
            if (fout[j] == f2[i]) { eout[j] += e2[i]; found = true; break; }
        }
        if (!found) { fout[out] = f2[i]; eout[out] = e2[i]; ++out; }
    }
    *outcnt = out;
    return out;
}

/* ------------------------------------------------------------
   Main sequence function
   ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;                 /* outside defined range */

    init_primes_and_spf();

    if (n > (idx_t)prime_count) return (seq_t)0; /* safety check */

    uint32_t p = primes[n - 1];

    /* Special case p = 2 */
    if (p == 2) return (seq_t)2;

    uint32_t a1 = p - 1;      /* p-1  */
    uint32_t a2 = p + 1;      /* p+1  */

    /* Factorisation containers: at most 20 distinct primes per factor */
    uint32_t f1[20], e1[20];
    uint32_t f2[20], e2[20];
    size_t cnt1, cnt2;

    factor_small(a1, f1, e1, &cnt1);
    factor_small(a2, f2, e2, &cnt2);

    /* Merge factor lists */
    uint32_t allf[40];
    unsigned allex[40];
    size_t allcnt;
    merge_factors(f1, e1, cnt1, f2, e2, cnt2, allf, allex, &allcnt);

    /* Count divisors: product of (exponent+1) */
    seq_t dcount = (seq_t)1;
    for (size_t i = 0; i < allcnt; ++i) {
        dcount *= (seq_t)(allex[i] + 1);
    }
    return dcount;
}
