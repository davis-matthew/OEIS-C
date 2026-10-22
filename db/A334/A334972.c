/*
 * A334972  Bi-unitary admirable numbers: numbers k such that there is a
 *          proper bi-unitary divisor d of k such that bsigma(k) - 2*d = 2*k,
 *          where bsigma is the sum of bi-unitary divisors function (A188999).
 * https://oeis.org/A334972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 1738, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, is_biunitary_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence:  A334972  Bi‑unitary admirable numbers
   (numbers k such that there exists a proper bi‑unitary divisor d of k
    with bsigma(k) – 2*d = 2*k, where bsigma is the sum of bi‑unitary
    divisors).  The function below searches integers, caching results
    for subsequent calls. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"   /* factor_u64(n, primes, exps, &cnt) */
#include "../utils/is_biunitary_divisor.h"

/* ------------------------------------------------------------------ */
/* Cache for the sequence values                                         */
static seq_t *seq          = NULL;   /* array of computed terms          */
static size_t seq_len      = 0;      /* number of values stored          */
static size_t seq_cap      = 0;      /* current capacity of the array    */
static uint64_t last_checked = 0;    /* last integer examined (k)        */

/* ------------------------------------------------------------------ */
/* Compute bsigma(k) – sum of bi‑unitary divisors of k                  */
/*    rule: for each prime with even exponent a, omit the divisor with
 *         exponent a/2                                   */
static seq_t bsigma_uint64(uint64_t n)
{
    if (n == 1)
        return (seq_t)1;   /* σ**(1) = 1 */

    /* factorisation of n */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t result = 1;     /* multiplicative identity */

    for (size_t i = 0; i < cnt; ++i) {
        int a = (int)exps[i];
        seq_t part = (seq_t)1;   /* start with p⁰ term   */
        seq_t pow   = (seq_t)1;   /* current power p⁰      */

        /* geometric series 1 + p + p² + ... + pᵃ           */
        for (int j = 1; j <= a; ++j) {
            pow  *= (seq_t)primes[i];
            part += pow;
        }

        /* remove middle term if a is even                  */
        if ((a & 1) == 0) {
            seq_t half_pow = (seq_t)1;
            for (int j = 0; j < a / 2; ++j)
                half_pow *= (seq_t)primes[i];
            part -= half_pow;
        }

        result *= part;
    }

    return result;
}

/* ------------------------------------------------------------------ */
/* Append a new term to the cached sequence                           */
static void push_seq(uint64_t val)
{
    if (seq_len + 1 > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 64;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
    seq[seq_len++] = (seq_t)val;
}

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Main entry point: return the n‑th term (1‑based index)             */
seq_t a(idx_t n)
{
    if (n == 0)                /* undefined, but keep safe          */
        return (seq_t)0;

    /* extend the cache until it contains the requested index       */
    while (seq_len < (size_t)n) {
        uint64_t k = ++last_checked;                   /* next candidate   */

        /* compute bsigma(k) */
        seq_t bs = bsigma_uint64(k);
        if (bs % 2 != 0)                                /* bs must be even  */
            continue;

        seq_t half = bs / 2;
        seq_t d_seq = half - (seq_t)k;                 /* candidate d    */
        if (d_seq == 0 || d_seq >= (seq_t)k)            /* properness?    */
            continue;

        uint64_t d = (uint64_t)d_seq;
        if (k % d != 0)                                 /* divisor needed  */
            continue;

        /* factorise k and d once each                      */
        uint64_t pk[64], ek[64], pd[64], ed[64];
        size_t cnt_k = 0, cnt_d = 0;
        factor_u64(k, pk, ek, &cnt_k);
        factor_u64(d, pd, ed, &cnt_d);

        /* verify bi‑unitary condition                       */
        if (!is_biunitary_divisor(k, pk, ek, cnt_k, pd, ed, cnt_d))
            continue;

        /* Found a bi‑unitary admirable number             */
        push_seq(k);
    }

    return seq[n - 1];
}
