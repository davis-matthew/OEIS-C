/*
 * A303205  Number of rectangles with squarefree area and dimensions p and
 *          |q-p| such that n = p + q and p < q.
 * https://oeis.org/A303205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor((n-1)/2)} mu(i*(n-2*i))^2
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Global table of Möbius values */
static signed char *mu_arr = NULL;
static idx_t mu_len = 0;

/* Ensure mu_arr is computed up to at least N (inclusive) */
static void ensure_mu(idx_t N)
{
    if (N < mu_len)
        return;

    /* free previous table if any */
    if (mu_arr) free(mu_arr);

    idx_t len = N + 1;
    mu_arr = (signed char *)malloc(len * sizeof(*mu_arr));
    if (!mu_arr) exit(1);

    if (len > 0) mu_arr[0] = 0;
    if (len > 1) mu_arr[1] = 1;

    if (N >= 2) {
        unsigned char *is_comp = (unsigned char *)calloc(len, sizeof(*is_comp));
        if (!is_comp) exit(1);

        /* primes list */
        size_t primes_cap = N / 10 + 10;
        int *primes = (int *)malloc(primes_cap * sizeof(*primes));
        if (!primes) exit(1);
        size_t primes_cnt = 0;

        for (idx_t i = 2; i <= N; ++i) {
            if (!is_comp[i]) {
                if (primes_cnt == primes_cap) {
                    primes_cap *= 2;
                    int *tmp = (int *)realloc(primes, primes_cap * sizeof(*primes));
                    if (!tmp) exit(1);
                    primes = tmp;
                }
                primes[primes_cnt++] = (int)i;
                mu_arr[i] = -1;
            }
            for (size_t j = 0; j < primes_cnt; ++j) {
                int p = primes[j];
                idx_t v = i * (idx_t)p;
                if (v > N) break;
                is_comp[v] = 1;
                if (i % p == 0) {
                    mu_arr[v] = 0;
                    break;
                } else {
                    mu_arr[v] = -mu_arr[i];
                }
            }
        }

        free(is_comp);
        free(primes);
    }

    mu_len = len;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* safety check for n=0 */

    idx_t lim = (n - 1) / 2;
    idx_t tmax = 0;

    /* Find maximum t = i*(n-2*i) for i in [1,lim] */
    for (idx_t i = 1; i <= lim; ++i) {
        idx_t t = i * (n - 2 * i);
        if (t > tmax) tmax = t;
    }

    ensure_mu(tmax);

    seq_t count = 0;
    for (idx_t i = 1; i <= lim; ++i) {
        idx_t t = i * (n - 2 * i);
        if (mu_arr[t] != 0)
            count += 1;
    }

    return count;
}
