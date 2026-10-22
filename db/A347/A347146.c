/*
 * A347146  a(n) = Sum_{d|n} (d^d)', where ' is the arithmetic derivative.
 * https://oeis.org/A347146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (d^d)' = Sum_{d|n} d^d * d'
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where d' = derivative of d computed via d' = j + p * d' for i = p*j */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/big_pow.h"

static idx_t *spf = NULL;          /* smallest prime factor array */
static size_t spf_size = 0;

static seq_t *der_cache = NULL;    /* derivative cache: der_cache[i] = i' */
static size_t der_size = 0;

/* Ensure that spf[0..n] and der_cache[0..n] are computed */
static void ensure_values(idx_t n)
{
    if (n <= spf_size)
        return;

    /* recompute spf up to n */
    spf = (idx_t *)realloc(spf, (n + 1) * sizeof(idx_t));
    for (idx_t i = 0; i <= n; ++i)
        spf[i] = i;
    for (idx_t i = 2; i * i <= n; ++i) {
        if (spf[i] == i) {
            for (idx_t j = i * i; j <= n; j += i)
                if (spf[j] == j)
                    spf[j] = i;
        }
    }

    /* extend derivative cache */
    der_cache = (seq_t *)realloc(der_cache, (n + 1) * sizeof(seq_t));
    der_cache[0] = 0;
    der_cache[1] = 0;
    for (idx_t i = 2; i <= n; ++i) {
        idx_t p = spf[i];
        idx_t j = i / p;
        der_cache[i] = (seq_t)j + (seq_t)p * der_cache[j];
    }

    spf_size = n;
    if (der_size < (size_t)(n + 1))
        der_size = n + 1;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    ensure_values(n);

    seq_t sum = 0;
    idx_t d;
    for (d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d2 = n / d;
            seq_t pow_d  = big_pow((seq_t)d,  (seq_t)d);
            sum += pow_d * der_cache[d];

            if (d2 != d) {
                seq_t pow_d2 = big_pow((seq_t)d2, (seq_t)d2);
                sum += pow_d2 * der_cache[d2];
            }
        }
    }
    return sum;
}
