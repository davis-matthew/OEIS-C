/*
 * A393993  Largest number of distinct slopes of lines determined by pairs of
 *          cell centers of a polyomino with n cells.
 * https://oeis.org/A393993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n + max_{1 <= k <= n-2} C(k-1, n-k-2) where C(a,b) = |{
 *            (p,q) : 1 <= q <= a, 1 <= p <= b, gcd(p,q)=1 }|
 * Range:     u8 n <= 18, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* cache for the Möbius function μ */
static int *mu_cache = NULL;                /* μ[d] for 0 <= d <= mu_max */
static idx_t mu_max = 0;                    /* largest d stored */

static void compute_mobius(idx_t N)
{
    if (N <= mu_max) return;                /* already computed */

    size_t size = (size_t)(N + 1);
    /* allocate new mus */
    int *newmu = (int *)malloc(size * sizeof(int));
    if (!newmu) return;                    /* allocation failure */
    /* integer array for composite flags */
    int *is_comp = (int *)calloc(size, sizeof(int));
    if (!is_comp) { free(newmu); return; }

    int *primes = (int *)malloc(size * sizeof(int));
    if (!primes) { free(newmu); free(is_comp); return; }

    int prime_cnt = 0;
    newmu[0] = 0;
    if (N >= 1) newmu[1] = 1;

    for (int i = 2; i <= (int)N; ++i) {
        if (!is_comp[i]) {
            primes[prime_cnt++] = i;
            newmu[i] = -1;
        }
        for (int j = 0; j < prime_cnt; ++j) {
            int p = primes[j];
            long long prod = 1LL * i * p;
            if (prod > N) break;
            is_comp[prod] = 1;
            if (i % p == 0) {
                newmu[prod] = 0;
                break;
            } else {
                newmu[prod] = -newmu[i];
            }
        }
    }

    free(is_comp);
    free(primes);

    /* replace old cache */
    free(mu_cache);
    mu_cache = newmu;
    mu_max = N;
}

/* compute number of coprime pairs (p,q) with 1<=q<=A, 1<=p<=B */
static seq_t coprime_pairs(idx_t A, idx_t B)
{
    if (A == 0 || B == 0) return 0;

    idx_t minAB = (A < B) ? A : B;
    long long sum = 0;
    /* μ values are stored in mu_cache up to mu_max >= minAB */
    for (idx_t d = 1; d <= minAB; ++d) {
        int mu_val = mu_cache[d];
        if (!mu_val) continue;
        idx_t qa = A / d;
        idx_t qb = B / d;
        sum += (long long)mu_val * qa * qb;
    }
    return (seq_t)sum;
}

/* externally visible routine – required by the judge */
seq_t a(idx_t n)
{
    if (n == 1) return 0;
    if (n == 2) return 1;

    /* ensure μ is available up to n */
    compute_mobius(n);

    seq_t max_cross = 0;

    for (idx_t k = 1; k <= n - 2; ++k) {
        idx_t denom_max = k - 1;               /* number of possible denominators (q) */
        idx_t numer_max = n - k - 2;           /* number of possible numerators (p) */

        seq_t cross = coprime_pairs(denom_max, numer_max);
        if (cross > max_cross) max_cross = cross;
    }

    return (seq_t)n + max_cross;
}
