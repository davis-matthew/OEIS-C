/*
 * A354157  Numerator of generalized Catalan number c_3(n) (see Comments).
 * https://oeis.org/A354157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of the generalized Catalan number c_3(n) c_3(n)
 *            = (1/3)*(1/(n+1/3))*∏_{i=0}^{n-1}(n+i+1/3)/n!; The sequence is
 *            the numerator of the reduced fraction whose denominator is a
 *            power of 3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Prime sieve and smallest prime factor table up to 60001. */
    static bool init = false;
    static uint32_t *primes = NULL;
    static unsigned short *prime_index = NULL;
    static uint32_t *spf = NULL;
    static size_t prime_cnt = 0;
    const idx_t LIMIT = 60001;                 /* covers all needed values */

    if (!init) {
        init = true;
        /* smallest prime factor */
        spf = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
        for (idx_t i = 0; i <= LIMIT; ++i) spf[i] = (uint32_t)i;
        for (idx_t i = 2; i * i <= LIMIT; ++i) {
            if (spf[i] == i) {
                for (idx_t j = i * i; j <= LIMIT; j += i)
                    if (spf[j] == j) spf[j] = (uint32_t)i;
            }
        }
        /* collect primes and build index lookup */
        size_t cnt = 0;
        for (idx_t i = 2; i <= LIMIT; ++i) if (spf[i] == i) ++cnt;
        prime_cnt = cnt;
        primes = (uint32_t *)malloc(prime_cnt * sizeof(uint32_t));
        prime_index = (unsigned short *)malloc((LIMIT + 1) *
                                               sizeof(unsigned short));
        for (idx_t i = 0; i <= LIMIT; ++i) prime_index[i] = 65535u; /* invalid */
        idx_t id = 0;
        for (idx_t i = 2; i <= LIMIT; ++i) if (spf[i] == i) {
            primes[id] = (uint32_t)i;
            prime_index[i] = (unsigned short)id;
            ++id;
        }
    }

    /* exponent table for the primes */
    int *exp = (int *)calloc(prime_cnt, sizeof(int));

    /* accumulate exponents of ∏_{j=n}^{2n-1}(3j+1) */
    idx_t start = n;
    idx_t end   = 2 * n;                     /* exclusive */
    for (idx_t j = start; j < end; ++j) {
        uint32_t v = (uint32_t)(3 * j + 1);
        while (v > 1) {
            uint32_t p = spf[v];
            unsigned short pos = prime_index[p];
            int cnt = 0;
            do {
                v /= p;
                ++cnt;
            } while (v % p == 0);
            exp[pos] += cnt;
        }
    }

    /* subtract exponents of (3n+1) */
    uint32_t v = (uint32_t)(3 * n + 1);
    while (v > 1) {
        uint32_t p = spf[v];
        unsigned short pos = prime_index[p];
        int cnt = 0;
        do {
            v /= p;
            ++cnt;
        } while (v % p == 0);
        exp[pos] -= cnt;
    }

    /* subtract exponents from n! */
    for (size_t k = 0; k < prime_cnt; ++k) {
        uint32_t p = primes[k];
        idx_t pw = p;
        int add = 0;
        while (pw <= n) {
            add += (int)(n / pw);
            if (pw > n / p) break;          /* avoid overflow */
            pw *= p;
        }
        exp[k] -= add;
    }

    /* build the result from prime powers */
    seq_t result = (seq_t)1;
    for (size_t k = 0; k < prime_cnt; ++k) {
        int e = exp[k];
        if (e <= 0) continue;
        seq_t base = (seq_t)primes[k];
        seq_t pow  = (seq_t)1;
        seq_t b    = base;
        int ee = e;
        while (ee > 0) {
            if (ee & 1) pow *= b;
            b *= b;
            ee >>= 1;
        }
        result *= pow;
    }

    free(exp);
    return result;
}
