/*
 * A352806  Orders of the finite groups PSL_2(K) when K is a finite field with
 *          q = A246655(n) elements.
 * https://oeis.org/A352806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |PSL(2,q_n)| where q_n is the n-th prime power. For q
 *            odd, |PSL(2,q)| = q*(q^2-1)/2; for q even, |PSL(2,q)| =
 *            q*(q^2-1)
 * Range:     u8 n <= 4, u16 n <= 19, u32 n <= 338, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   fill_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/fill_spf.h"

/* Check if x is a prime power using its smallest prime factor table. */
static bool is_prime_power(idx_t x, idx_t *spf) {
    idx_t p = spf[x];
    while (x % p == 0) x /= p;
    return x == 1;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;             /* below offset */

    /* Rough upper bound for the n-th prime power: 20*n+100 suffices up to very large n. */
    idx_t limit = n > 0 ? n * 20 + 100 : 100;
    if (limit < 2) limit = 2;

    /* Allocate SPF array */
    idx_t *spf = (idx_t *)calloc((size_t)(limit + 1), sizeof(idx_t));
    if (!spf) return 0;               /* allocation failed */

    fill_spf(limit, spf);

    /* Scan numbers to find the n-th prime power */
    idx_t count = 0;
    idx_t q_value = 0;                 /* placeholder */
    for (idx_t x = 2; x <= limit; ++x) {
        if (is_prime_power(x, spf)) {
            ++count;
            if (count == n) {
                q_value = x;
                break;
            }
        }
    }

    /* If not found within the first bound, enlarge the limit and repeat */
    while (count < n) {
        /* Double the limit */
        idx_t old_limit = limit;
        limit = limit * 2 + 100;
        idx_t *new_spf = (idx_t *)calloc((size_t)(limit + 1), sizeof(idx_t));
        if (!new_spf) { free(spf); return 0; }
        /* Copy old spf values */
        for (idx_t i = 0; i <= old_limit; ++i) new_spf[i] = spf[i];
        /* Compute spf for the new range */
        for (idx_t i = old_limit + 1; i <= limit; ++i) new_spf[i] = 0;
        fill_spf(limit, new_spf);
        free(spf);
        spf = new_spf;

        /* Continue scanning from next odd numbers */
        for (idx_t x = old_limit + 1; x <= limit; ++x) {
            if (is_prime_power(x, spf)) {
                ++count;
                if (count == n) {
                    q_value = x;
                    break;
                }
            }
        }
    }

    free(spf);

    /* Now compute |PSL(2, q_value)| */
    seq_t q = (seq_t)q_value;
    seq_t qsq = q * q;
    seq_t prod = q * (qsq - (seq_t)1);
    seq_t order;
    if (q_value & 1) {
        order = prod / (seq_t)2;
    } else {
        order = prod;
    }
    return order;
}
