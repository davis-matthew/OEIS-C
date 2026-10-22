/*
 * A368405  Infinitary version of Mertens's function: a(n) = Sum_{k=1..n}
 *          A064179(k).
 * https://oeis.org/A368405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = ∑_{k=1..n} A064179(k) where A064179(k) = μ_inf(k) =
 *            (-1)^{∑ popcount(exponent in k’s prime factorisation)}
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* count number of set bits in an idx_t value  */
static int popcount_uint64(idx_t v) {
    int c = 0;
    while (v) {
        c += v & 1;
        v >>= 1;
    }
    return c;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;              /* undefined for n < 1, just return 0 */

    /* Smallest prime factor array */
    idx_t *spf = (idx_t*)malloc((n + 1) * sizeof(idx_t));
    if (!spf) return 0;                /* allocation failure */
    for (idx_t i = 0; i <= n; ++i) spf[i] = 0;
    spf[1] = 1;

    for (idx_t i = 2; i <= n; ++i) {
        if (!spf[i]) {                 /* i is prime, spf[i] = i later */
            for (idx_t j = i; j <= n; j += i) {
                if (!spf[j]) spf[j] = i;
            }
        }
    }

    /* μ_inf array */
    seq_t *mu = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!mu) { free(spf); return 0; }
    mu[1] = 1;

    for (idx_t i = 2; i <= n; ++i) {
        idx_t p = spf[i];
        idx_t x = i;
        idx_t exp = 0;
        while (x % p == 0) {
            ++exp;
            x /= p;
        }
        int pc = popcount_uint64(exp);
        seq_t sign = (pc & 1) ? -1 : 1;   /* (-1)^popcount(exp) */
        mu[i] = sign * mu[x];
    }

    /* cumulative sum */
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) sum += mu[i];

    free(spf);
    free(mu);
    return sum;
}
