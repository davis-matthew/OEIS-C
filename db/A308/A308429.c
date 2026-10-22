/*
 * A308429  Take all the integer-sided triangles with perimeter n and
 *          nonsquarefree side lengths a, b and c such that a <= b <= c. a(n)
 *          is the sum of all the b's.
 * https://oeis.org/A308429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{a=1}^{⌊n/3⌋} Σ_{b=a}^{⌊(n−a)/2⌋} * [a + b > n−a−b] ·
 *            (μ(a)=0) · (μ(b)=0) · (μ(n−a−b)=0) · b * where μ(x) is the
 *            Möbius function. Only non‑squarefree sides * contribute, i.e.
 *            μ(x)=0. The function returns the sum of all * middle sides b for
 *            integer‑sided triangles with perimeter n
 * Range:     u8 n <= 67, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static state for μ values */
static idx_t mu_limit = 0;
static int *mu_arr   = NULL;

static void ensure_mu(idx_t n)
{
    if (mu_limit >= n) return;          /* already computed */

    /* Compute μ up to n using linear sieve */
    idx_t N = n;
    int *mu = (int *)malloc((N + 1) * sizeof(int));
    if (!mu) exit(EXIT_FAILURE);        /* unlikely for small N */

    /* prime list and composite flags */
    int  *primes = (int *)malloc((N + 1) * sizeof(int));
    char *is_comp = (char *)calloc(N + 1, sizeof(char));
    int   pcnt = 0;

    mu[1] = 1;
    for (idx_t i = 2; i <= N; ++i) {
        if (!is_comp[i]) {                /* i is prime */
            primes[pcnt++] = (int)i;
            mu[i] = -1;
        }
        for (int j = 0; j < pcnt; ++j) {
            idx_t p = (idx_t)primes[j];
            idx_t ip = i * p;
            if (ip > N) break;
            is_comp[ip] = 1;
            if (i % p == 0) {
                mu[ip] = 0;
                break;
            } else {
                mu[ip] = -mu[i];
            }
        }
    }
    free(primes);
    free(is_comp);

    /* replace old array */
    free(mu_arr);
    mu_arr   = mu;
    mu_limit = N;
}

seq_t a(idx_t n)
{
    if (n < 3) return 0;                 /* no triangles possible */

    ensure_mu(n);

    seq_t sum = 0;
    idx_t max_a = n / 3;
    for (idx_t a_val = 1; a_val <= max_a; ++a_val) {
        idx_t max_b = (n - a_val) / 2;
        if (max_b < a_val) continue;
        for (idx_t b_val = a_val; b_val <= max_b; ++b_val) {
            idx_t c_val = n - a_val - b_val;
            if (c_val < b_val) continue;            /* ensures a<=b<=c */
            if (a_val + b_val <= c_val) continue;    /* triangle condition */
            if (mu_arr[a_val] == 0 && mu_arr[b_val] == 0 && mu_arr[c_val] == 0) {
                sum += (seq_t)b_val;
            }
        }
    }
    return sum;
}
