/*
 * A376846  Number of m <= n such that rad(m) | n and Omega(m) > Omega(n),
 *          where rad = A007947 and Omega = A001222.
 * https://oeis.org/A376846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{m ≤ n : rad(m) | n and Ω(m) > Ω(n)}|
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Shadowing the public types provided by the build */
typedef uint64_t idx_t;

/* Forward declarations of helpers */
static void build_spf(idx_t limit, idx_t **spf_out);
static void init_up_to(idx_t n);

/* Static cache of computed values */
static idx_t max_precomp = 0;
static idx_t *rad_arr   = NULL;
static idx_t *omega_arr = NULL;

/* Linear sieve to compute smallest prime factors up to limit */
static void build_spf(idx_t limit, idx_t **spf_out)
{
    idx_t *spf = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
    if (!spf) {
        *spf_out = NULL;
        return;
    }
    for (idx_t i = 0; i <= limit; ++i) spf[i] = 0;

    idx_t *primes = (idx_t *)malloc((limit / 2 + 1) * sizeof(idx_t));
    if (!primes) {
        free(spf);
        *spf_out = NULL;
        return;
    }
    size_t primes_cnt = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            primes[primes_cnt++] = i;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            idx_t p = primes[j];
            if (p > spf[i] || i * p > limit) break;
            spf[i * p] = p;
        }
    }

    free(primes);
    *spf_out = spf;
}

/* Populate rad_arr and omega_arr up to n (inclusive) */
static void init_up_to(idx_t n)
{
    if (n <= max_precomp) return;

    idx_t *spf = NULL;
    build_spf(n, &spf);
    if (!spf) return;

    idx_t *new_rad   = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    idx_t *new_omega = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    if (!new_rad || !new_omega) {
        free(spf);
        free(new_rad);
        free(new_omega);
        return;
    }

    new_rad[0]  = 1;   /* unused but convenient */
    new_rad[1]  = 1;   /* rad(1) = 1 */
    new_omega[0] = 0;  /* unused */
    new_omega[1] = 0;  /* Ω(1) = 0 */

    for (idx_t i = 2; i <= n; ++i) {
        idx_t p   = spf[i];
        idx_t rem = i / p;
        new_rad[i]   = new_rad[rem];
        if (rem % p != 0) new_rad[i] *= p;
        new_omega[i] = new_omega[rem] + 1;
    }

    free(spf);
    free(rad_arr);
    free(omega_arr);

    rad_arr   = new_rad;
    omega_arr = new_omega;
    max_precomp = n;
}

/* Public entry point for OEIS sequence */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* sequence starts at n = 1 */
    init_up_to(n);

    idx_t omega_n = omega_arr[n];
    seq_t count = 0;

    for (idx_t m = 1; m <= n; ++m) {
        if ((n % rad_arr[m] == 0) && (omega_arr[m] > omega_n))
            ++count;
    }
    return count;
}
