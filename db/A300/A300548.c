/*
 * A300548  a(n) = [x^n] Product_{d|n} 1/(1 + x^d).
 * https://oeis.org/A300548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = [x^n] Product_{d|n} 1/(1 + x^d)
 * Range:     i8 n <= 44, i16 n <= 164, i32 n <= 824, i64 n <= 3464,
 *            i128 n <= 9547, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/*
 * Compute a(n) for any n ≥ 0.
 * Complexity: O(n * τ(n)) – fast enough for n up to about 10 000.
 */
seq_t a(idx_t n)
{
    if (n == 0ULL)
        return (seq_t)1;

    /* ----- Factorise n to obtain its prime factors and exponents ----- */
    idx_t temp = n;
    idx_t primes[64];
    int   exps[64];
    int   pc = 0;        /* number of distinct prime factors */

    if ((temp & 1U) == 0) {
        primes[pc] = 2;
        exps[pc]   = 0;
        while ((temp & 1U) == 0) {
            temp >>= 1U;
            exps[pc]++;
        }
        pc++;
    }
    for (idx_t p = 3; p * p <= temp; p += 2U) {
        if (temp % p == 0) {
            primes[pc] = p;
            exps[pc]   = 0;
            while (temp % p == 0) {
                temp /= p;
                exps[pc]++;
            }
            pc++;
        }
    }
    if (temp > 1U) {            /* last prime factor */
        primes[pc] = temp;
        exps[pc]   = 1;
        pc++;
    }

    /* ----- Generate all divisors of n ----- */
    idx_t divs[256];
    int   divcnt = 1;
    divs[0] = 1U;

    for (int i = 0; i < pc; ++i) {
        idx_t p = primes[i];
        int   e = exps[i];
        int   curcnt = divcnt;
        idx_t mult = 1U;
        for (int k = 1; k <= e; ++k) {
            mult *= p;
            for (int j = 0; j < curcnt; ++j)
                divs[divcnt++] = divs[j] * mult;
        }
    }

    /* ----- Build the coefficient of x^n by iterating divisors ----- */
    size_t sz = (size_t)(n + 1);
    seq_t *dp   = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *next = (seq_t*)calloc(sz, sizeof(seq_t));
    if (!dp || !next) {          /* allocation failure – graceful exit */
        free(dp);
        free(next);
        return (seq_t)0;
    }

    dp[0] = (seq_t)1;
    for (size_t i = 1; i < sz; ++i)
        dp[i] = (seq_t)0;

    for (int ix = 0; ix < divcnt; ++ix) {
        idx_t d = divs[ix];          /* divisor */
        next[0] = dp[0];
        for (size_t j = 1; j < sz; ++j) {
            if (j < d) {
                next[j] = dp[j];
            } else {
                next[j] = dp[j] - next[j - d];
            }
        }
        seq_t *tmp = dp;
        dp = next;
        next = tmp;
    }

    seq_t result = dp[(size_t)n];
    free(dp);
    free(next);
    return result;
}
