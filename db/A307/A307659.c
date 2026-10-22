/*
 * A307659  G.f. A(x) satisfies: (1 + x)/(1 - x) =
 *          A(x)*A(x^2)^2*A(x^3)^3*A(x^4)^4* ... *A(x^k)^k* ...
 * https://oeis.org/A307659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   The sequence satisfies a(0)=1 and for n≥1 * a(n) = (1/n) *
 *            Σ_{k=1}^n 2 * S(k) * a(n-k), * where S(k)= Σ_{d|k, (k/d) odd}
 *            μ(d) * d², * and μ is the Möbius function. * This follows from
 *            the logarithm of the generating function * A(x)=∏_{k≥1}
 *            ((1+x^k)/(1−x^k))^(μ(k)·k)
 * Range:     i8 n <= 5, i16 n <= 18, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Compute the sequence value a(n). */
seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* Allocate space for Möbius function and a small temporary array. */
    size_t N = (size_t)n + 1;
    int *mu        = (int*)malloc(N * sizeof(int));
    idx_t *pr      = (idx_t*)malloc(N * sizeof(idx_t));
    bool *iscomp   = (bool*)calloc(N, sizeof(bool));

    /* Linear sieve to compute μ up to n. */
    mu[0] = 0;           /* unused */
    mu[1] = 1;
    size_t pcnt = 0;
    for (idx_t i = 2; i <= n; ++i) {
        if (!iscomp[i]) {
            pr[pcnt++] = i;
            mu[i] = -1;           /* first power of a prime has μ = -1 */
        }
        for (size_t j = 0; j < pcnt; ++j) {
            idx_t p = pr[j];
            idx_t v = i * p;
            if (v > n) break;
            iscomp[v] = true;
            if (i % p == 0) {     /* p divides i */
                mu[v] = 0;
                break;
            } else {
                mu[v] = -mu[i];
            }
        }
    }

    /* Allocate arrays for S(k) and the sequence a(k). */
    seq_t *S   = (seq_t*)malloc(N * sizeof(seq_t));
    seq_t *arr = (seq_t*)malloc(N * sizeof(seq_t));

    /* Compute S(k) = Σ_{d|k, (k/d) odd} μ(d) * d². */
    for (idx_t i = 0; i < N; ++i) S[i] = 0;
    for (idx_t d = 1; d <= n; ++d) {
        int mud = mu[d];
        if (mud == 0) continue;
        for (idx_t m = 1; d * m <= n; m += 2) {   /* m odd => k/d odd */
            idx_t k = d * m;
            S[k] += (seq_t)mud * d * d;
        }
    }

    /* Compute the sequence a(k). */
    arr[0] = 1;
    for (idx_t cur = 1; cur <= n; ++cur) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= cur; ++k) {
            /* term = 2 * S(k) * a(cur - k) */
            sum += (seq_t)2 * S[k] * arr[cur - k];
        }
        /* Since sum is always divisible by cur, integer division is exact. */
        arr[cur] = sum / (seq_t)cur;
    }

    seq_t result = arr[n];

    /* Free all allocated memory. */
    free(mu);
    free(pr);
    free(iscomp);
    free(S);
    free(arr);

    return result;
}
