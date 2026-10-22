/*
 * A332595  Number of triangular regions in a "frame" of size n X n (see
 *          Comments in A331776 for definition), divided by 4.
 * https://oeis.org/A332595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*( Σ_{1≤i,j≤n , gcd(i,j)=2} (n+1-i)(n+1-j) + (n+3)*(n-1)
 *            )
 * Range:     u8 n <= 6, u16 n <= 29, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n = 1, the value is 1.  See the derivation in the OEIS entry. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* All arithmetic uses signed long long to keep intermediate values
       in a range that fits in 64 bits.  The final result is cast to seq_t. */
    long long n_ll = (long long)n;
    long long n1   = n_ll + 1;          /* (n+1) */
    long long m    = n_ll / 2;           /* floor(n/2) */

    /* Allocate working arrays */
    int    *mu   = (int *)malloc((m + 1) * sizeof(int));
    int    *is_comp = (int *)malloc((m + 1) * sizeof(int));
    int    *primes = (int *)malloc((m + 1) * sizeof(int));
    long long *phi = (long long *)malloc((m + 1) * sizeof(long long));
    long long *sum_b = (long long *)malloc((m + 1) * sizeof(long long));
    long long *f = (long long *)malloc((m + 1) * sizeof(long long));
    long long *g = (long long *)malloc((m + 1) * sizeof(long long));

    if (!mu || !is_comp || !primes || !phi || !sum_b || !f || !g) {
        /* Allocation failed – return 0 as a safe fallback. */
        free(mu); free(is_comp); free(primes); free(phi); free(sum_b);
        free(f); free(g);
        return (seq_t)0;
    }

    /* Linear sieve to compute μ[1..m] */
    mu[1] = 1;
    int pc = 0;
    for (long long i = 2; i <= m; ++i) {
        if (!is_comp[i]) {
            primes[pc++] = (int)i;
            mu[i] = -1;
        }
        for (int j = 0; j < pc; ++j) {
            long long p = primes[j];
            long long ip = i * p;
            if (ip > m) break;
            is_comp[ip] = 1;
            if (i % p == 0) {
                mu[ip] = 0;
                break;
            } else {
                mu[ip] = -mu[i];
            }
        }
    }

    /* Pre‑compute f[d] and g[d] for all d */
    for (long long d = 1; d <= m; ++d) {
        f[d] = m / d;               /* floor(m/d) */
        g[d] = f[d] * (f[d] + 1);   /* t*(t+1) */
        phi[d] = 0;
        sum_b[d] = 0;
    }

    /* Convolution using Möbius values */
    for (long long d = 1; d <= m; ++d) {
        if (mu[d] == 0) continue;
        long long val_phi   = mu[d] * f[d];
        long long val_sum_b = mu[d] * d * g[d];
        for (long long a = d; a <= m; a += d) {
            phi[a]   += val_phi;
            sum_b[a] += val_sum_b;
        }
    }

    /* Compute z_2(n) */
    long long z2 = 0;
    for (long long a = 1; a <= m; ++a) {
        long long Aa = n1 - 2 * a;                 /* (n+1-2a) */
        long long Sa = n1 * phi[a] - sum_b[a];     /* S(a) */
        z2 += Aa * Sa;
    }

    /* Remaining part (n+3)*(n-1) */
    long long aenc = (n_ll + 3) * (n_ll - 1);

    long long result = 2 * (z2 + aenc);

    /* Clean up */
    free(mu); free(is_comp); free(primes);
    free(phi); free(sum_b); free(f); free(g);

    return (seq_t)result;
}
