/*
 * A374119  a(n) = 1 if A113177(n) and A276085(n) are both multiples of 3,
 *          otherwise 0, where A113177 and A276085 are fully additive with
 *          a(p) = Fibonacci(p) and a(p) = p#/p, respectively.
 * https://oeis.org/A374119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if A113177(n) and A276085(n) are both multiples of 3,
 *            otherwise 0
 * Range:     u8 n <= 9946, u16 n <= 9946, u32 n <= 9946, u64 n <= 9946,
 *            u128 n <= 9946, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_spf.h"

static idx_t *spf          = NULL;      /* smallest prime factor array      */
static idx_t  spf_limit    = 0;          /* current allocated limit          */
static idx_t *fib_mod3     = NULL;      /* Fibonacci(p) mod 3               */
static bool    ready       = false;     /* initialization flag              */

/* Ensure spf and fib_mod3 are allocated up to at least n */
static void ensure_limit(idx_t n)
{
    if (!ready || n > spf_limit) {
        /* free previous allocations if any */
        if (spf)    free(spf);
        if (fib_mod3) free(fib_mod3);

        /* build new spf array up to n */
        build_spf(n, &spf);                 /* spf[0] unused, spf[1] = 1   */

        /* allocate fib_mod3 array (mod 3 values) */
        fib_mod3 = (idx_t *)malloc((size_t)(n + 1) * sizeof(idx_t));
        if (!fib_mod3) exit(EXIT_FAILURE);
        fib_mod3[0] = 0;
        fib_mod3[1] = 1;
        for (idx_t i = 2; i <= n; ++i)
            fib_mod3[i] = (fib_mod3[i - 1] + fib_mod3[i - 2]) % 3;

        spf_limit = n;
        ready     = true;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                 /* undefined for n < 1 */

    ensure_limit(n);

    /* n = 1 has no prime factors; both sums are 0 → multiple of 3 */
    if (n == 1)
        return (seq_t)1;

    idx_t temp   = n;
    idx_t sum1   = 0;   /* sum of e * Fib(p)   (mod 3)   */
    idx_t sum2   = 0;   /* sum of e * (primorial(p)/p) (mod 3) */

    while (temp > 1) {
        idx_t p   = spf[temp];
        idx_t cnt = 0;
        while (temp % p == 0) {
            temp /= p;
            ++cnt;
        }
        idx_t cnt_mod = cnt % 3;
        if (cnt_mod == 0)
            continue;

        /* A113177 contribution */
        sum1 = (sum1 + cnt_mod * fib_mod3[p]) % 3;

        /* A276085 contribution – only primes 2 and 3 give non‑zero mod 3 */
        if (p == 2)
            sum2 = (sum2 + cnt_mod) % 3;                   /* coeff 1   */
        else if (p == 3)
            sum2 = (sum2 + cnt_mod * 2) % 3;               /* coeff 2   */
        /* primes ≥5 contribute 0 mod 3, skip */
    }

    return (seq_t)((sum1 == 0 && sum2 == 0) ? 1 : 0);
}
