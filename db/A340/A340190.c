/*
 * A340190  Möbius transform of A063994(x) = Product_{primes p dividing x}
 *          gcd(p-1, x-1).
 * https://oeis.org/A340190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} μ(n/d) * A063994(d) where A063994(x) =
 *            Product_{p|x} gcd(p-1, x-1) and μ is the Möbius function
 * Range:     i8 n <= 129, i16 n <= 8191, i32 n <= 8191, i64 n <= 8191,
 *            i128 n <= 8191, big any n
 * Helpers:   build_spf, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/build_spf.h"

/* Smallest prime factor table (spf[0..limit]) */
static idx_t *spf = NULL;       /* spf array */
static idx_t spf_max = 0;       /* highest n for which spf is valid */

/* Ensure spf covers 2..n, rebuilding if necessary. */
static void ensure_spf(idx_t n)
{
    if (n <= spf_max) return;
    free(spf);
    build_spf(n, &spf);
    spf_max = n;
}

/* Möbius μ(m) using the precomputed spf array.  m ≥ 1. */
static int mu_of_idx(idx_t m)
{
    if (m == 1) return 1;
    int prime_cnt = 0;
    while (m > 1) {
        idx_t p = spf[m];
        int exp = 0;
        while (m % p == 0) {
            m /= p;
            exp++;
            if (exp > 1) return 0;   /* squared prime factor → 0 */
        }
        prime_cnt++;
    }
    return (prime_cnt & 1) ? -1 : 1;
}

/* Compute A063994(x) = product over distinct primes dividing x of gcd(p-1, x-1). */
static seq_t A063994(idx_t x)
{
    if (x <= 1) return (seq_t)1;   /* empty product */
    seq_t prod = 1;
    idx_t temp = x;
    idx_t last_p = 0;
    while (temp > 1) {
        idx_t p = spf[temp];
        if (p != last_p) {
            seq_t g = gcd((seq_t)(p - 1), (seq_t)(x - 1));
            prod *= g;
            last_p = p;
        }
        temp /= p;
    }
    return prod;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset starts at 1 */
    ensure_spf(n);

    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            idx_t d2 = n / d;
            int mu1 = mu_of_idx(n / d1);          /* μ(n/d1) */
            sum += (seq_t)mu1 * A063994(d1);
            if (d2 != d1) {
                int mu2 = mu_of_idx(n / d2);      /* μ(n/d2) */
                sum += (seq_t)mu2 * A063994(d2);
            }
        }
    }
    return sum;
}
