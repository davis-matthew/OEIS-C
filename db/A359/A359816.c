/*
 * A359816  Parity of A359815, where A359815 is the Dirichlet inverse of
 *          A359770, which is the characteristic function for numbers k such
 *          that k and bigomega(k) are of different parity.
 * https://oeis.org/A359816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9606, u16 n <= 9606, u32 n <= 9606, u64 n <= 9606,
 *            u128 n <= 9606, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A359816: parity of the Dirichlet inverse of A359770.
   A359770(k) = 1 if k and bigomega(k) have different parity, 0 otherwise.
   a(n) is the parity (0/1) of the Dirichlet inverse value at n. */

#include <stdlib.h>
#include <stdint.h>

/* static cache for up to the largest n requested. */
static idx_t  max_n = 0;
static uint64_t *spf       = NULL;   /* smallest prime factor */
static uint8_t  *bigomParity = NULL;/* Ω(n) mod 2 */
static uint8_t  *fParity    = NULL;  /* A359770 parity array */
static uint8_t  *gParity    = NULL;  /* Dirichlet inverse parity array */

/* Allocate (or enlarge) all arrays up to 'limit' (1‑based). */
static void ensure_capacity(idx_t limit)
{
    if (limit <= max_n) return;

    spf       = (uint64_t *)realloc(spf,       sizeof(uint64_t) * (limit + 1));
    bigomParity = (uint8_t *)realloc(bigomParity, sizeof(uint8_t)  * (limit + 1));
    fParity   = (uint8_t *)realloc(fParity,   sizeof(uint8_t)  * (limit + 1));
    gParity   = (uint8_t *)realloc(gParity,   sizeof(uint8_t)  * (limit + 1));

    /* Smallest prime factor sieve (1‑based). */
    if (limit >= 1) spf[1] = 1;
    for (idx_t i = 2; i <= limit; ++i) spf[i] = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (i > limit / i) continue;
            for (idx_t j = i * i; j <= limit; j += i)
                if (spf[j] == 0) spf[j] = i;
        }
    }

    /* Ω(n) mod 2 via recurrence Ω(1)=0. */
    bigomParity[1] = 0;
    for (idx_t i = 2; i <= limit; ++i)
        bigomParity[i] = bigomParity[i / spf[i]] ^ 1;

    /* f(n) = 1 if n and Ω(n) parity differ, otherwise 0. */
    for (idx_t i = 1; i <= limit; ++i)
        fParity[i] = (uint8_t)((i & 1) ^ bigomParity[i]);

    /* Dirichlet inverse parity: g(1)=1, g(n)=Σ_{d|n, d<n} g(d)*f(n/d) mod 2. */
    gParity[1] = 1;
    for (idx_t n = 2; n <= limit; ++n) {
        uint8_t acc = 0;
        for (idx_t d = 1; (idx_t)d * d <= n; ++d) {
            if (n % d == 0) {
                idx_t d1 = d;
                idx_t d2 = n / d;
                if (d1 < n) {
                    acc ^= gParity[d1] & fParity[d2];
                }
                if (d2 < n && d2 != d1) {
                    acc ^= gParity[d2] & fParity[d1];
                }
            }
        }
        gParity[n] = acc;
    }

    max_n = limit;
}

/* External function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety – offset is 1 */
    ensure_capacity(n);
    return (seq_t)gParity[n];
}
