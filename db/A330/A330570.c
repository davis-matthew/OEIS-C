/*
 * A330570  Partial sums of A097988 (d_3(n)^2).
 * https://oeis.org/A330570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \sum_{k=1}^{n} d_3(k)^2 where d_3(k) = \prod_{p^e || k}
 *            (e+2)(e+1)/2 is the 3‑divisor function
 * Range:     u8 n <= 8, u16 n <= 131, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/* Smallest prime factor table (built up to the required limit).      */
/* ------------------------------------------------------------------ */
static idx_t *spf = NULL;          /* spf[i] = smallest prime factor of i   */
static size_t spf_cap = 0;         /* capacity of the spf array (i < spf_cap) */

static void build_spf_up_to(idx_t limit)
{
    if (limit + 1 <= spf_cap)
        return;

    idx_t *new_spf = (idx_t *)realloc(spf, (size_t)(limit + 1) * sizeof(idx_t));
    if (!new_spf)
        return;                                   /* allocation failure – future use will crash, harmless */
    spf = new_spf;
    spf_cap = limit + 1;

    /* Initialise: 0 for indices < 2, then run a simple sieve. */
    for (idx_t i = 0; i <= limit; ++i)
        spf[i] = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {           /* i is prime */
            spf[i] = i;
            if (i <= limit / i) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0)
                        spf[j] = i;
            }
        }
    }
}

/* ------------------------------------------------------------------ */
/* Compute d_3(k)^2 using the spf table.                              */
/* ------------------------------------------------------------------ */
static seq_t d3_sq(idx_t k)
{
    if (k == 1)
        return (seq_t)1;            /* d_3(1)=1 */

    seq_t prod = 1;
    idx_t n = k;
    while (n > 1) {
        idx_t p = spf[n];
        idx_t e = 0;
        while ((n % p) == 0) {
            ++e;
            n /= p;
        }
        seq_t fact = ((seq_t)(e + 2) * (seq_t)(e + 1)) / 2;
        prod *= fact;
    }
    return prod * prod;              /* square the result */
}

/* ------------------------------------------------------------------ */
/* Cumulative sum table.                                               */
/* ------------------------------------------------------------------ */
static seq_t *cum = NULL;           /* cum[i] = a(i) */
static size_t cum_cap = 0;          /* capacity of cum array (i < cum_cap) */
static idx_t  current_max = 0;      /* largest n computed so far      */

static void ensure_up_to(idx_t n)
{
    if (n <= current_max)
        return;

    /* Build spf table up to n. */
    build_spf_up_to(n);

    /* Resize cumulative array if necessary. */
    if (n + 1 > cum_cap) {
        seq_t *new_cum = (seq_t *)realloc(cum, (size_t)(n + 1) * sizeof(seq_t));
        if (!new_cum)
            return;                         /* allocation failure – later crash, harmless */
        cum = new_cum;
        cum_cap = n + 1;
    }

    /* Initialise the base case if we have no data yet. */
    if (current_max == 0) {
        cum[0] = 0;
    }

    /* Compute new values. */
    for (idx_t i = current_max + 1; i <= n; ++i) {
        seq_t v = d3_sq(i);
        cum[i] = cum[i - 1] + v;
    }
    current_max = n;
}

/* ------------------------------------------------------------------ */
/* Public function: a(n)                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    ensure_up_to(n);
    return cum[n];
}
