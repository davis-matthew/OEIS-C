/*
 * A397622  a(n) = prime(A005653(n)).
 * https://oeis.org/A397622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(A005653(n)), where A005653 lists the positive
 *            integers k with frac(k*phi) < 1/2, phi = (1+sqrt(5))/2; computed
 *            by a search plus a sieve
 * Range:     u8 n <= 27, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static idx_t A005653_a(idx_t n) {
    const double phi = (1.0 + sqrt(5.0)) / 2.0;
    idx_t count = 0;
    for (idx_t k = 1; ; k++) {
        double product = (double)k * phi;
        double frac = product - floor(product);
        if (frac < 0.5) {
            count++;
            if (count == n) {
                return k;
            }
        }
    }
}

static uint64_t *A397622_primes = NULL;
static size_t A397622_count = 0;
static uint64_t A397622_limit = 0;

static void A397622_ensure(idx_t n)
{
    while ((idx_t)A397622_count < n) {
        uint64_t lim = A397622_limit < 64 ? 64 : A397622_limit * 2;
        unsigned char *comp = (unsigned char *)calloc((size_t)lim + 1, 1);
        uint64_t i, j;
        size_t cnt = 0;
        uint64_t *pr;
        if (!comp)
            return;
        for (i = 2; i * i <= lim; i++)
            if (!comp[i])
                for (j = i * i; j <= lim; j += i)
                    comp[j] = 1;
        for (i = 2; i <= lim; i++)
            if (!comp[i])
                cnt++;
        pr = (uint64_t *)malloc(cnt * sizeof(uint64_t));
        if (!pr) {
            free(comp);
            return;
        }
        cnt = 0;
        for (i = 2; i <= lim; i++)
            if (!comp[i])
                pr[cnt++] = i;
        free(comp);
        free(A397622_primes);
        A397622_primes = pr;
        A397622_count = cnt;
        A397622_limit = lim;
    }
}

seq_t a(idx_t n)
{
    idx_t k;
    if (n < 1)
        return (seq_t)0;
    k = A005653_a(n);
    A397622_ensure(k);
    if ((idx_t)A397622_count < k)
        return (seq_t)0;
    return (seq_t)A397622_primes[k - 1];
}
