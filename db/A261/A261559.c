/*
 * A261559  Denominator of Product_{k=1..n} (2k/(2k+1))^((-1)^A000120(k)).
 * https://oeis.org/A261559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   Denominator of Π_{k=1..n} (2k/(2k+1))^((-1)^popcount(k)) * after
 *            reducing the fraction to lowest terms
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* helper: linear sieve for smallest prime factor up to `limit`. */
static void build_spf(uint32_t *spf, size_t limit)
{
    for (size_t i = 0; i <= limit; ++i)
        spf[i] = (uint32_t)i;
    if (limit >= 2)
        for (size_t i = 2; i * i <= limit; ++i)
            if (spf[i] == (uint32_t)i)
                for (size_t j = i * i; j <= limit; j += i)
                    if (spf[j] == (uint32_t)j)
                        spf[j] = (uint32_t)i;
}

/* Factor `num` using spf and update exponent array `exp`. */
static void factor_and_update(uint32_t num, int sign,
                              uint32_t *spf, int *exp)
{
    while (num > 1) {
        uint32_t p = spf[num];
        int cnt = 0;
        while (num % p == 0) {
            num /= p;
            ++cnt;
        }
        exp[p] += sign * cnt;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)         /* undefined for n=0 – return 0 safely */
        return (seq_t)0;

    /* upper bound for sieve */
    size_t limit = 2 * (size_t)n + 1;

    /* allocate/resize smallest prime factor table */
    static uint32_t *spf = NULL;
    static size_t spf_cap = 0;
    if (limit + 1 > spf_cap) {
        free(spf);
        spf = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
        if (!spf) exit(1);
        spf_cap = limit + 1;
    }
    build_spf(spf, limit);

    /* allocate exponent array */
    static int *exp = NULL;
    static size_t exp_cap = 0;
    if (limit + 1 > exp_cap) {
        free(exp);
        exp = (int *)malloc((limit + 1) * sizeof(int));
        if (!exp) exit(1);
        exp_cap = limit + 1;
    }
    for (size_t i = 0; i <= limit; ++i) exp[i] = 0;

    /* main loop over k */
    for (idx_t k = 1; k <= n; ++k) {
        int e = (((__builtin_popcountll((uint64_t)k) & 1) == 0) ? 1 : -1);
        uint32_t t = 2u * (uint32_t)k;
        factor_and_update(t, e, spf, exp);          /* 2k contributes +e */
        t = 2u * (uint32_t)k + 1u;
        factor_and_update(t, -e, spf, exp);         /* 2k+1 contributes -e */
    }

    /* compute denominator from negative prime exponents */
    seq_t result = (seq_t)1;
    for (size_t p = 2; p <= limit; ++p) {
        if (exp[p] < 0) {
            int e = -exp[p];
            for (int i = 0; i < e; ++i)
                result *= (seq_t)p;
        }
    }
    return result;
}
