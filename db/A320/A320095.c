/*
 * A320095  Number of primitive (=aperiodic) n-ary words with length less than
 *          or equal to n which are earlier in lexicographic order than any
 *          other word derived by cyclic shifts of the alphabet.
 * https://oeis.org/A320095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d=1..n} n^{d-1} * (Sum_{k=1..⌊n/d⌋} mu(k))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (derived from a(n) = Sum_{j=1..n} Sum_{d|j} n^{d-1} * mu(j/d)) */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety – seq_t is unsigned */

    /* -------------------------------------------------------
       1. Compute Möbius mu[1..n] with a linear sieve
    ------------------------------------------------------- */
    size_t size = (size_t)n + 1;    /* indices 0..n */
    int32_t *mu = (int32_t *)malloc(size * sizeof(int32_t));
    int    *pr = (int    *)malloc(size * sizeof(int));
    char   *is_comp = (char  *)calloc(size, sizeof(char));
    if (!mu || !pr || !is_comp) {
        free(mu); free(pr); free(is_comp);
        return 0;                    /* allocation failed */
    }

    mu[0] = 0;
    if (n >= 1) mu[1] = 1;
    int primes_cnt = 0;

    for (idx_t i = 2; i <= n; ++i) {
        if (!is_comp[(size_t)i]) {
            pr[primes_cnt++] = (int)i;
            mu[(size_t)i] = -1;
        }
        for (int j = 0; j < primes_cnt; ++j) {
            int p = pr[j];
            size_t prod = (size_t)i * (size_t)p;
            if (prod > n) break;
            is_comp[prod] = 1;
            if (i % p == 0) {
                mu[prod] = 0;
                break;
            } else {
                mu[prod] = -mu[(size_t)i];
            }
        }
    }

    /* -------------------------------------------------------
       2. Prefix sums of μ:  M[k] = Sum_{t=1..k} μ(t)
    ------------------------------------------------------- */
    int32_t *M = (int32_t *)malloc(size * sizeof(int32_t));
    if (!M) {
        free(mu); free(pr); free(is_comp);
        return 0;
    }
    M[0] = 0;
    for (idx_t i = 1; i <= n; ++i) {
        M[(size_t)i] = M[(size_t)(i - 1)] + mu[(size_t)i];
    }

    /* -------------------------------------------------------
       3. Accumulate a(n) using the derived formula
    ------------------------------------------------------- */
    seq_t pos = 0, neg = 0;           /* keep track of sign */
    seq_t power = 1;                  /* n^{d-1} */
    for (idx_t d = 1; d <= n; ++d) {
        idx_t idx = n / d;            /* ⌊n/d⌋ */
        int32_t mval = M[(size_t)idx];
        if (mval == 0) {
            /* nothing to add */
        } else {
            seq_t term = power * (seq_t)(mval >= 0 ? mval : -mval);
            if (mval > 0) pos += term;
            else          neg += term;
        }
        /* prepare next power */
        power *= (seq_t)n;
    }

    /* -------------------------------------------------------
       4. Result and cleanup
    ------------------------------------------------------- */
    free(mu); free(pr); free(is_comp); free(M);

    if (pos >= neg) return pos - neg;
    /* The algebra guarantees pos >= neg for all n >= 1 */
    return 0;
}
