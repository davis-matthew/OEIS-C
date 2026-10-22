/*
 * A323868  Number of matrices of size n whose entries cover an initial
 *          interval of positive integers.
 * https://oeis.org/A323868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000005(n) * A000670(n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* If compiled as C++ with arbitrary–precision seq_t,
   use the utils that properly construct seq_t objects. */
#ifdef __cplusplus
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"
#endif

/* ------------------------------------------------------------------
   Smallest prime factor sieve for unsigned 32‑bit numbers
   ------------------------------------------------------------------ */
static uint32_t *spf = NULL;      /* spf[2..] */
static size_t   spf_size = 0;     /* array capacity */

/* Ensure spf[2..n] are filled */
static void ensure_spf(idx_t n)
{
    if (spf_size <= (size_t)n) {
        spf = (uint32_t*)realloc(spf, (n + 1) * sizeof(uint32_t));
        spf_size = n + 1;
    }
    for (idx_t i = 2; i <= n; ++i) spf[i] = 0;

    for (idx_t i = 2; i <= n; ++i) {
        if (spf[i] == 0) {                /* i is prime */
            spf[i] = (uint32_t)i;
            if ((uint64_t)i * i <= (uint64_t)n) {
                for (idx_t j = i * i; j <= n; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }
}

/* ------------------------------------------------------------------
   Divisor count table
   ------------------------------------------------------------------ */
static seq_t *d      = NULL;      /* d[1..] */
static size_t d_size = 0;        /* array capacity */
static idx_t   d_last = 0;        /* largest index computed */

/* Ensure d[1..n] are computed */
static void ensure_divisors(idx_t n)
{
    if (n <= d_last) return;

    /* enlarge array if needed */
#ifdef __cplusplus
    if (d_size <= (size_t)n) {
        seq_t *newp = alloc_seq_array(n + 1);
        if (d) {
            for (size_t i = 0; i < d_size; ++i) newp[i] = d[i];
            free_seq_array(d);
        }
        d = newp;
        d_size = n + 1;
    }
#else
    if (d_size <= (size_t)n) {
        d = (seq_t*)realloc(d, (n + 1) * sizeof(seq_t));
        d_size = n + 1;
    }
#endif

    /* compute divisor counts */
    for (idx_t i = d_last + 1; i <= n; ++i) {
        seq_t cnt = 1;
        idx_t m = i;
        while (m > 1) {
            uint32_t p = spf[m];
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            cnt *= (seq_t)(e + 1);
        }
        d[i] = cnt;
    }
    d_last = n;
}

/* ------------------------------------------------------------------
   Fubini (ordered Bell) numbers table
   ------------------------------------------------------------------ */
static seq_t *f      = NULL;      /* f[0..] */
static size_t f_size = 0;        /* array capacity */
static idx_t   f_last = 0;        /* largest index computed */

/* Ensure f[0..n] are computed */
static void ensure_fubini(idx_t n)
{
    if (n <= f_last) return;

    /* enlarge array if needed */
#ifdef __cplusplus
    if (f_size <= (size_t)n) {
        seq_t *newp = alloc_seq_array(n + 1);
        if (f) {
            for (size_t i = 0; i < f_size; ++i) newp[i] = f[i];
            free_seq_array(f);
        }
        f = newp;
        f_size = n + 1;
    }
#else
    if (f_size <= (size_t)n) {
        f = (seq_t*)realloc(f, (n + 1) * sizeof(seq_t));
        f_size = n + 1;
    }
#endif

    /* initialise f[0] if not already done */
    if (f_last == 0 && f_size >= 1) f[0] = (seq_t)1;

    for (idx_t i = f_last + 1; i <= n; ++i) {
        seq_t sum = 0;
        seq_t bin = 1;                       /* C(i,0) */
        for (idx_t k = 0; k < i; ++k) {
            sum += bin * f[k];
            if (k + 1 < i) {
                bin = bin * (seq_t)(i - k) / (seq_t)(k + 1);
            }
        }
        f[i] = sum;
    }
    f_last = n;
}

/* ------------------------------------------------------------------
   Main sequence function
   ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset starts at 1 */

    ensure_spf(n);
    ensure_divisors(n);
    ensure_fubini(n);

    return d[n] * f[n];
}
