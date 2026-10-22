/*
 * A303997  Number of ways to write 2*n as p + 3^k + binomial(2*m,m), where p
 *          is a prime, and k and m are nonnegative integers.
 * https://oeis.org/A303997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write 2*n as p + 3^k + C(2*m,m) where *
 *            p is prime and k,m are non‑negative integers
 * Range:     u8 n <= 233, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* cache for primes, powers of 3 and central binomial coefficients */
static idx_t current_maxN = 0;
static uint8_t *prime_flags = NULL;          /* 1 = prime, 0 = composite */
static idx_t *pow3_array = NULL;             /* 3^k values */
static idx_t pow3_count = 0;
static seq_t *bino_array = NULL;            /* C(2*m,m) values */
static idx_t bino_count = 0;

/* Recompute tables up to required n */
static void recompute(idx_t n)
{
    current_maxN = n;
    idx_t limit = 2 * n;

    /* primes up to limit */
    if (prime_flags) free(prime_flags);
    prime_flags = (uint8_t *)calloc((size_t)limit + 1, sizeof(uint8_t));
    for (idx_t i = 0; i <= limit; ++i) prime_flags[i] = 1;
    if (limit >= 0) prime_flags[0] = 0;
    if (limit >= 1) prime_flags[1] = 0;
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (prime_flags[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                prime_flags[j] = 0;
        }
    }

    /* powers of 3 */
    if (pow3_array) free(pow3_array);
    pow3_array = (idx_t *)calloc(64, sizeof(idx_t));  /* ample size */
    pow3_count = 0;
    idx_t v = 1;
    while (v <= limit) {
        pow3_array[pow3_count++] = v;
        if (v > limit / 3) break;
        v *= 3;
    }

    /* central binomial coefficients C(2*m,m) */
    if (bino_array) free(bino_array);
    bino_array = (seq_t *)calloc(64, sizeof(seq_t));  /* ample size */
    bino_count = 0;
    seq_t bino = 1;
    bino_array[bino_count++] = bino;              /* m = 0 */
    for (idx_t m = 1; ; ++m) {
        bino = bino * (seq_t)(4 * m - 2) / (seq_t)m;
        if (bino > (seq_t)limit) break;
        bino_array[bino_count++] = bino;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at n = 1 */

    if (n > current_maxN) recompute(n);

    seq_t target = (seq_t)2 * (seq_t)n;
    seq_t count = 0;

    for (idx_t ki = 0; ki < pow3_count; ++ki) {
        seq_t residual = target - (seq_t)pow3_array[ki];
        if (residual < 2) continue;   /* no prime possible */
        for (idx_t mi = 0; mi < bino_count; ++mi) {
            seq_t binom = bino_array[mi];
            if (binom > residual) break;
            seq_t p = residual - binom;
            if (p < 2) continue;
            if (prime_flags[(idx_t)p]) ++count;
        }
    }

    return count;
}
