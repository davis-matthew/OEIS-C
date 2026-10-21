/*
 * A277379  E.g.f.: exp(x/(1-x^2))/sqrt(1-x^2).
 * https://oeis.org/A277379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |H_n((1+i)/2)|^2 / 2^n Recurrence:
 *            (n+1)(n+2)(a(n)-n^2*a(n-1)) + (2n^2+7n+6)a(n+1) + a(n+2) =
 *            a(n+3) Initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=10
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t cache[10001];   /* enough for n ≤ 10000 */
static idx_t cache_max = 0;  /* highest index whose value is known */

seq_t a(idx_t n)
{
    /* initialise first four terms */
    if (cache_max == 0) {
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 2;
        cache[3] = 10;
        cache_max = 3;
    }

    if (n <= cache_max) return cache[n];

    /* compute successive terms up to n */
    for (idx_t m = cache_max + 1; m <= n; ++m) {
        if (m < 4) continue;                    /* safety, real code never hits this */

        idx_t k = m - 3;                       /* current n in the recurrence   */
        seq_t k_seq   = (seq_t)k;
        seq_t k_plus_1 = k_seq + 1;
        seq_t k_plus_2 = k_seq + 2;
        seq_t k_sq     = k_seq * k_seq;

        /* term1 = (k+1)(k+2)*a(k) */
        seq_t term1 = k_plus_1 * k_plus_2 * cache[k];

        /* coeff2 = 2k^2 + 7k + 6 ; term2 = coeff2 * a(k+1) */
        seq_t coeff2 = 2 * k_sq + 7 * k_seq + 6;
        seq_t term2 = coeff2 * cache[k + 1];

        /* term3 = a(k+2)   */
        seq_t term3 = cache[k + 2];

        /* sum of the three positive contributions */
        seq_t sum = term1 + term2 + term3;

        /* term4 = k^2*(k+1)*(k+2)*a(k-1)  (subtracted) */
        seq_t term4 = k_sq * k_plus_1 * k_plus_2 * cache[k - 1];

        /* a(k+3) = sum - term4 */
        cache[m] = sum - term4;
    }

    cache_max = n;
    return cache[n];
}
