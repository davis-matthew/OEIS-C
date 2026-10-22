/*
 * A353691  a(n) is the least number k > n such that h(k)/h(n) is an integer,
 *          where h(n) is the harmonic mean of the divisors of n, or -1 if no
 *          such k exists.
 * https://oeis.org/A353691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k > n such that h(k)/h(n) is an integer, where h(m)
 *            = m·d(m)/σ(m) is the harmonic mean of the divisors of m. d(m) =
 *            number of divisors, σ(m) = sum of divisors
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 31, u64 n <= 639, u128 n <= 639,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Smallest prime factor table */
static uint32_t *spf = NULL;
static uint32_t spf_limit = 0;

/* Initialise or resize the spf table up to 'limit' (inclusive).  */
static void init_spf(uint32_t limit)
{
    /* free old table if present */
    if (spf != NULL) {
        free(spf);
    }
    spf = (uint32_t *)malloc((size_t)(limit + 1u) * sizeof(uint32_t));
    if (spf == NULL) abort();
    spf_limit = limit;

    /* initialise */
    for (uint32_t i = 0; i <= limit; ++i) {
        spf[i] = i;
    }

    /* sieve */
    for (uint32_t i = 2; (uint64_t)i * i <= limit; ++i) {
        if (spf[i] == i) {                     /* i is prime */
            for (uint32_t j = i * i; j <= limit; j += i) {
                if (spf[j] == j) spf[j] = i;
            }
        }
    }
}

/* Factor 'n' (uint32_t) into its divisors count d and divisor sum σ.
   The results are returned in *d_out and *sigma_out as seq_t. */
static void factor_sigma_d(uint32_t n, seq_t *sigma_out, seq_t *d_out)
{
    seq_t d_acc = 1;
    seq_t sigma_acc = 1;
    uint32_t nn = n;

    while (nn > 1) {
        uint32_t p = spf[nn];
        uint32_t exp = 0;
        while (nn % p == 0) {
            nn /= p;
            ++exp;
        }

        /* Compute σ(p^exp) = 1 + p + ... + p^exp */
        seq_t term = 1;
        seq_t power = 1;
        for (uint32_t k = 0; k < exp; ++k) {
            power *= (seq_t)p;
            term += power;
        }

        sigma_acc *= term;
        d_acc *= (uint64_t)(exp + 1);
    }

    *sigma_out = sigma_acc;
    *d_out      = d_acc;
}

seq_t a(idx_t n)
{
    /* Initialise spf table on first call. */
    static int init_done = 0;
    if (!init_done) {
        init_spf(5000000u);        /* enough for n ≤ 10 000 and typical searches */
        init_done = 1;
    }

    /* Ensure n is within the spf table */
    if ((uint32_t)n > spf_limit) {
        init_spf((uint32_t)(n * 2u + 1u));
    }

    seq_t sigma_n, d_n;
    factor_sigma_d((uint32_t)n, &sigma_n, &d_n);

    uint64_t k = (uint64_t)n + 1ul;
    for (;;) {
        /* Extend spf table if needed */
        if (k > spf_limit) {
            init_spf((uint32_t)(k * 2u + 1u));
        }

        seq_t sigma_k, d_k;
        factor_sigma_d((uint32_t)k, &sigma_k, &d_k);

        /* numerator = k * d_k * sigma_n */
        /* denominator = n * d_n * sigma_k */
        seq_t num = (seq_t)k * d_k;
        num *= sigma_n;
        seq_t den = (seq_t)n * d_n;
        den *= sigma_k;

        if (num % den == (seq_t)0) {
            return (seq_t)k;
        }

        ++k;
    }

    /* Unreachable, but some compilers need a return */
    return (seq_t)-1;
}
