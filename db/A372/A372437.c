/*
 * A372437  (Least binary index of n) minus (least prime index of n).
 * https://oeis.org/A372437
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = A001511(n) – A055396(n) where A001511(n) = position of
 *            the least significant 1‑bit in n (trailing_zeros(n)+1) and
 *            A055396(n) = index of the smallest prime divisor of n
 * Range:     i8 n <= 72, i16 n <= 72, i32 n <= 72, i64 n <= 72, i128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* ------------- state for the sieve ------------------------------------ */
static uint64_t *spf        = NULL;   /* smallest prime factor */
static uint32_t *prime_index= NULL;   /* prime index for each prime  */
static idx_t     max_pre    = 0;      /* largest n precomputed          */

/* ------------- helpers ----------------------------------------------- */

/* count trailing zeros of an unsigned 64‑bit integer   */
static inline idx_t trailing_zeros(idx_t x)
{
    idx_t c = 0;
    while ((x & 1) == 0)
    {
        ++c;
        x >>= 1;
    }
    return c;
}

/* compute spf and prime_index up to N (inclusive) */
static void precompute(idx_t N)
{
    if (N <= max_pre)
        return;

    /* resize arrays */
    spf         = (uint64_t*)realloc(spf,        (N + 1)*sizeof(uint64_t));
    prime_index = (uint32_t*)realloc(prime_index, (N + 1)*sizeof(uint32_t));

    /* initialise */
    for (idx_t i = 0; i <= N; ++i)
    {
        spf[i]        = 0;
        prime_index[i] = 0;
    }

    /* sieve for smallest prime factor */
    for (idx_t i = 2; i <= N; ++i)
    {
        if (spf[i] == 0)
        {
            spf[i] = i;
            if (i > N / i)   /* avoid overflow of i*i */
                continue;
            for (idx_t j = i * i; j <= N; j += i)
            {
                if (spf[j] == 0)
                    spf[j] = i;
            }
        }
    }

    /* assign prime indices */
    uint32_t cnt = 0;
    for (idx_t i = 2; i <= N; ++i)
    {
        if (spf[i] == i)          /* i is prime */
        {
            ++cnt;
            prime_index[i] = cnt;  /* 2 -> 1, 3 -> 2, ... */
        }
    }

    max_pre = N;
}

/* ------------- requested function ------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 2)        /* offset starts at 2, value is irrelevant below */
        return 0;

    /* ensure sieve data covers n */
    precompute(n);

    /* least binary index: trailing zeros + 1 */
    idx_t lt = trailing_zeros(n) + 1;

    /* smallest prime factor of n */
    uint64_t sp = spf[n];
    /* prime index of that prime factor */
    uint32_t pk = prime_index[sp];

    /* difference in seq_t */
    return (seq_t)lt - (seq_t)pk;
}
