/*
 * A392384  Numbers k such that sigma(k) = psi(k) + tau(k) + omega(k)^6.
 * https://oeis.org/A392384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, full, fini, changed
 * Formula:   numbers k such that sigma(k) = psi(k) + tau(k) + omega(k)^6
 *            (search). The function a(idx_t n) returns the n‑th term of this
 *            sequence. It builds the values in ascending order and memoises
 *            the results so that arbitrary calls to a() are fast
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* compute omega^6 (omega is the number of distinct prime factors) */
static inline seq_t pow6(seq_t x)
{
    seq_t r = 1;
    for (int i = 0; i < 6; ++i) r *= x;
    return r;
}

/* test whether a given integer k satisfies the defining equation */
static bool ok(uint64_t k)
{
    /* factorise k */
    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_u64(k, pr, ex, &cnt);

    seq_t sigma = 1;
    seq_t tau   = 1;
    seq_t psi   = (seq_t)k;
    seq_t omega = (seq_t)cnt;        /* number of distinct primes */

    for (size_t i = 0; i < cnt; ++i)
    {
        /* tau = product (e+1) */
        tau *= (seq_t)(ex[i] + 1);

        /* sigma = product (p^(e+1)-1)/(p-1) = product sum_{j=0}^e p^j */
        seq_t term = 1;
        seq_t powp = 1;
        for (uint64_t j = 0; j < ex[i]; ++j)
        {
            powp *= (seq_t)pr[i];
            term += powp;
        }
        sigma *= term;

        /* psi = n * product (p+1)/p; update in integer arithmetic */
        psi *= (seq_t)(pr[i] + 1);
        psi /= (seq_t)pr[i];
    }

    seq_t left  = sigma;
    seq_t right = psi + tau + pow6(omega);
    return left == right;
}

/* memoised sequence storage */
static seq_t *seq          = NULL;   /* array of sequence values */
static size_t seq_cap      = 0;      /* allocated capacity */
static size_t seq_len      = 0;      /* number of values stored */
static uint64_t last_checked = 0;    /* last integer examined */

/* append a new value to the memoised list */
static void seq_push_back(seq_t v)
{
    if (seq_len == seq_cap)
    {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        seq = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
    seq[seq_len++] = v;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset starts at 1; below is arbitrary */

    /* return memoised value if already computed */
    if ((size_t)n <= seq_len)
    {
        return seq[(size_t)n - 1];
    }

    uint64_t k = last_checked + 1;
    while (seq_len < (size_t)n)
    {
        if (ok(k))
        {
            seq_push_back((seq_t)k);
        }
        ++k;
    }
    last_checked = k - 1;
    return seq[(size_t)n - 1];
}
