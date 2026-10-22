/*
 * A364713  a(n) is the numerator of coefficient of x^n in expansion of (1 +
 *          x)^(1/n).
 * https://oeis.org/A364713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = numerator of binomial(1/n, n) for n ≥ 1 (OEIS A364713)
 * Range:     i8 n <= 4, i16 n <= 5, i32 n <= 9, i64 n <= 14, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* compute a(n) ----------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, safety */

    /* ---------- prime sieve up to n ----------------------------------- */
    bool *isComposite = (bool *)malloc((n + 1) * sizeof(bool));
    idx_t *primes      = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    for (idx_t i = 0; i <= n; ++i) isComposite[i] = false;
    idx_t pc = 0;                         /* number of primes found */
    for (idx_t i = 2; i <= n; ++i) {
        if (!isComposite[i]) {
            primes[pc++] = i;
            if (i * i <= n) {
                for (idx_t j = i * i; j <= n; j += i)
                    isComposite[j] = true;
            }
        }
    }
    free(isComposite);

    /* ---------- denominator exponents ---------------------------------- */
    int *denomExp = NULL;
    if (pc > 0) {
        denomExp = (int *)calloc(pc, sizeof(int));
        for (idx_t pi = 0; pi < pc; ++pi) {
            idx_t p = primes[pi];
            /* exponent of p in n */
            idx_t tmp = n, e = 0;
            while (tmp % p == 0) { ++e; tmp /= p; }
            if (e) denomExp[pi] += (int)(e * n);   /* from n^n */

            /* exponent of p in n! via Legendre */
            idx_t m = n;
            e = 0;
            while (m) { m /= p; e += m; }
            denomExp[pi] += (int)e;
        }
    }

    /* ---------- numerator exponents ------------------------------------ */
    int *numExp = NULL;
    if (pc > 0) numExp = (int *)calloc(pc, sizeof(int));

    seq_t result = 1;                     /* hold primes > n */

    for (idx_t k = 0; k < n; ++k) {
        seq_t val = (seq_t)1;
        if (k != 0)
            val = (seq_t)( (seq_t)1 - (seq_t)k * (seq_t)n );
        if (val < 0) val = -val;          /* work with absolute value */
        if (val == 1) continue;            /* no effect */

        idx_t tmp = (idx_t)val;
        /* factor with primes <= sqrt(tmp) */
        for (idx_t pi = 0; pi < pc && primes[pi] * primes[pi] <= tmp; ++pi) {
            idx_t p = primes[pi];
            int cnt = 0;
            while (tmp % p == 0) { ++cnt; tmp /= p; }
            if (cnt) {
                if (pc > 0) numExp[pi] += cnt;
            }
        }
        if (tmp > 1) {                     /* remaining prime factor */
            if (tmp <= n) {
                /* find its index in primes array */
                idx_t pos = 0;
                while (pos < pc && primes[pos] < tmp) ++pos;
                if (pos < pc && primes[pos] == tmp) {
                    if (pc > 0) numExp[pos] += 1;
                } else {
                    /* should not happen, fall back */
                    result *= (seq_t)tmp;
                }
            } else {
                result *= (seq_t)tmp;       /* prime > n stays in result */
            }
        }
    }

    /* ---------- cancel common factors ------------------------------- */
    if (pc > 0) {
        for (idx_t pi = 0; pi < pc; ++pi) {
            int leftover = numExp[pi] - denomExp[pi];
            if (leftover > 0) {
                idx_t p = primes[pi];
                for (int t = 0; t < leftover; ++t)
                    result *= (seq_t)p;
            }
        }
    }

    /* ---------- apply sign ------------------------------------------------ */
    seq_t sign = ((n & 1) == 0) ? (seq_t)-1 : (seq_t)1;
    result *= sign;

    /* ---------- cleanup ---------------------------------------------------- */
    if (primes) free(primes);
    if (denomExp) free(denomExp);
    if (numExp) free(numExp);

    return result;
}
