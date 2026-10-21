/*
 * A295418  a(n) = n! * Laguerre(n, n*(n-1), -n).
 * https://oeis.org/A295418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} binomial(n^2,n-k)*n^k/k!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

#define MAX_FACT 10001   /* Enough for n up to 10000 */
static seq_t fact_cache[MAX_FACT];
static idx_t fact_max = 0;

seq_t a(idx_t n)
{
    /* Initialize factorial cache */
    if (fact_max == 0) {
        fact_cache[0] = (seq_t)1;
        fact_max = 0;
    }
    if (n > fact_max) {
        for (idx_t i = fact_max + 1; i <= n; ++i) {
            fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
        }
        fact_max = n;
    }

    seq_t fac_n = fact_cache[n];

    idx_t N = n * n;                 /* n squared, fits in idx_t */

    seq_t u = binomial(N, n) * fac_n;  /* first term of the sum */
    seq_t sum = u;

    for (idx_t k = 0; k < n; ++k) {
        idx_t r = n - k;
        seq_t denom = (seq_t)(N - r + 1) * ((seq_t)k + 1);
        seq_t numer = u * (seq_t)r * (seq_t)n;
        u = numer / denom;          /* exact division */
        sum += u;
    }

    return sum;
}
