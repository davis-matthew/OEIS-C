/*
 * A334082  a(n) is the number of binomial coefficients C(n,k), with 0<=k<=n,
 *          that are not practical.
 * https://oeis.org/A334082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   build_spf, is_practical
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts non‑practical binomial coefficients C(n,k), 0≤k≤n. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_spf.h"
#include "../utils/is_practical.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined but harmless */

    /* ---------- build smallest‑prime‑factor array up to n ---------- */
    idx_t *spf = NULL;
    build_spf(n, &spf);
    if (!spf)
        return 0; /* allocation failure guard */

    /* ---------- collect primes up to n ---------- */
    size_t pcnt = 0;
    for (idx_t i = 2; i <= n; ++i) {
        if (spf[i] == i)
            ++pcnt;
    }

    idx_t *primes = (idx_t *)malloc(pcnt * sizeof(idx_t));
    if (!primes) {
        free(spf);
        return 0;
    }

    idx_t *prime_to_index = (idx_t *)calloc((size_t)(n + 1), sizeof(idx_t));
    if (!prime_to_index) {
        free(primes);
        free(spf);
        return 0;
    }

    size_t pi = 0;
    for (idx_t i = 2; i <= n; ++i) {
        if (spf[i] == i) {
            primes[pi] = i;
            prime_to_index[i] = pi + 1;      /* store index+1 to distinguish 0 */
            ++pi;
        }
    }

    /* ---------- exponent array for current binomial coefficient ---------- */
    uint16_t *exp = (uint16_t *)calloc(pcnt, sizeof(uint16_t));
    if (!exp) {
        free(prime_to_index);
        free(primes);
        free(spf);
        return 0;
    }

    const seq_t maxPrime = (seq_t)(pcnt > 0 ? primes[pcnt - 1] : 0);

    seq_t bad = 0;     /* count of non‑practical coefficients */

    for (idx_t k = 0; k <= n; ++k) {
        if (!is_practical(exp, primes, pcnt, maxPrime))
            ++bad;

        if (k == n)
            break;

        /* ---------- update exponents to obtain C(n,k+1) ---------- */

        /* multiply by (n-k) */
        idx_t x = n - k;
        while (x > 1) {
            idx_t p = spf[x];
            uint16_t cnt = 0;
            while (x % p == 0) {
                x /= p;
                ++cnt;
            }
            size_t idx = prime_to_index[p] - 1;
            exp[idx] += cnt;
        }

        /* divide by (k+1) */
        x = k + 1;
        while (x > 1) {
            idx_t p = spf[x];
            uint16_t cnt = 0;
            while (x % p == 0) {
                x /= p;
                ++cnt;
            }
            size_t idx = prime_to_index[p] - 1;
            exp[idx] -= cnt;
        }
    }

    free(exp);
    free(prime_to_index);
    free(primes);
    free(spf);

    return bad;
}
