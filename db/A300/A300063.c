/*
 * A300063  Heinz numbers of integer partitions of odd numbers.
 * https://oeis.org/A300063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 124, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = n-th Heinz number of an integer partition of an
 * odd integer (weight).  The sequence is obtained by taking all
 * integer partitions of odd weight, computing the product of the
 * primes indexed by the partition parts, and sorting the resulting
 * numbers in increasing order.
 */

#ifndef A300063_H
#define A300063_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#ifdef __cplusplus
#  include <cstddef>
#  include <cstdint>
#  include <cstdlib>
#endif

/* Maximum limit for generating numbers.  At 2 000 000 about 1 000 000
   odd‑weight Heinz numbers are produced, which is far more than
   required for n ≤ 10 000. */
static const size_t LIMIT = 2000000;

static seq_t *heinz_seq = NULL;        /* array of computed values */
static size_t   heinz_len = 0;         /* number of values stored */

/* Helper to allocate integer arrays */
static void *alloc_array(size_t nelems, size_t elsize)
{
    void *p = malloc(nelems * elsize);
    if (!p) exit(EXIT_FAILURE);
    return p;
}

/* Initialisation routine (called automatically by a() on demand) */
static void init_heinz(void)
{
    if (heinz_seq) return;  /* already initialised */

    /* ---- prime sieve + SPF (smallest prime factor) ---- */
    uint32_t *spf     = (uint32_t *)alloc_array(LIMIT + 1, sizeof(uint32_t));
    uint8_t   *pparity = (uint8_t   *)alloc_array(LIMIT + 1, sizeof(uint8_t));  /* parity of prime index */
    uint8_t   *parity  = (uint8_t   *)alloc_array(LIMIT + 1, sizeof(uint8_t));  /* parity of weighted exponents */

    for (size_t i = 0; i <= LIMIT; ++i) spf[i] = (uint32_t)i;

    for (size_t i = 2; i * i <= LIMIT; ++i)
        if (spf[i] == (uint32_t)i)
            for (size_t j = i * i; j <= LIMIT; j += i)
                if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;

    /* prime parity array: 0 for composites, 1 or 0 for prime index mod 2 */
    size_t prime_count = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (spf[i] == (uint32_t)i)
            pparity[i] = (uint8_t)((++prime_count) & 1);
    /* composites already have 0 */

    /* ---- compute weighted‑exponent parity for each integer ---- */
    parity[1] = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
    {
        uint32_t p = spf[i];
        size_t  exp = 0;
        uint32_t x  = (uint32_t)i;
        while (x % p == 0) { x /= p; ++exp; }
        uint8_t contrib = (pparity[p] & (exp & 1));
        parity[i] = parity[x] ^ contrib;
    }

    /* ---- collect all odd‑weight Heinz numbers up to LIMIT ---- */
    size_t cnt = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (parity[i]) ++cnt;

    /* allocate array for values */
#ifdef __cplusplus
    heinz_seq = new seq_t[cnt];
#else
    heinz_seq = (seq_t *)malloc(cnt * sizeof(seq_t));
#endif
    heinz_len = cnt;
    if (!heinz_seq) exit(EXIT_FAILURE);

    size_t idx = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (parity[i]) heinz_seq[idx++] = (seq_t)i;

    /* free helper arrays */
    free(spf);
    free(pparity);
    free(parity);
}

/* ----------------------------------------------------------- */

seq_t a(idx_t n)
{
    init_heinz();
    /* assume callers never request n > heinz_len (>=10 000 is guaranteed) */
    return heinz_seq[n - 1];
}

#endif /* A300063_H */
