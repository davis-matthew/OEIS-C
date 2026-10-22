/*
 * A362936  a(n) = A000982(n) - A362931(n).
 * https://oeis.org/A362936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000982(n) - A362931(n)
 * Range:     u8 n <= 24, u16 n <= 365, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint32_t *spf = NULL;      /* smallest prime factor array */
static idx_t     spf_limit = 0;   /* current size of spf array   */

static void build_spf(idx_t limit)
{
    if (limit <= spf_limit)
        return;

    free(spf);
    spf = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    for (idx_t i = 0; i <= limit; ++i)
        spf[i] = (uint32_t)i;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (spf[i] == (uint32_t)i) {
            for (idx_t j = i * i; j <= limit; j += i) {
                if (spf[j] == (uint32_t)j)
                    spf[j] = (uint32_t)i;
            }
        }
    }
    spf_limit = limit;
}

seq_t a(idx_t n)
{
    /* Count of ordered pairs (i,j) with integral arithmetic mean */
    idx_t e = n / 2;          /* number of even elements in [1..n] */
    idx_t o = n - e;          /* number of odd  elements in [1..n] */
    seq_t arith = (seq_t)e * (seq_t)e + (seq_t)o * (seq_t)o;

    /* Count of ordered pairs (i,j) with integral harmonic mean   */
    seq_t harm = 0;
    idx_t maxS = n * 2;                     /* maximum possible sum i+j */
    build_spf(maxS);

    for (idx_t s = 2; s <= maxS; ++s) {
        /* For a given sum s, find the minimal P such that
           s | 2*i^2  iff  i is a multiple of P.                 */
        idx_t g = (s & 1) ? 1 : 2;           /* gcd(s,2) */
        idx_t t = s / g;                    /* need t | i^2       */
        idx_t temp = t;
        idx_t P = 1;                        /* minimal multiplier  */
        while (temp > 1) {
            idx_t p = spf[temp];
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            idx_t need = (exp + 1) / 2;     /* ceil(exp/2) */
            for (idx_t k = 0; k < need; ++k)
                P *= p;
        }

        /* i must satisfy s-i between 1 and n, and 1 ≤ i ≤ n   */
        idx_t L = (s > n) ? (s - n) : 1;     /* lower bound for i   */
        idx_t R = (s - 1 < n) ? (s - 1) : n; /* upper bound for i   */
        if (L > R)
            continue;

        idx_t cnt = R / P - (L - 1) / P;
        harm += (seq_t)cnt;
    }

    return arith - harm;
}
