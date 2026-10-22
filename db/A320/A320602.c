/*
 * A320602  Irregular table read by rows: T(n,k) = (k+s)^(k+s) mod n, s =
 *          lcm(n, A002322(n)) = A174824(n), 0 <= k <= s - 1.
 * https://oeis.org/A320602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(m,k) where s(m) = lcm(m, λ(m)), λ(m) = Carmichael
 *            function of m, T(n,k) = (k+s)^(k+s) mod n, m is the smallest
 *            integer with cum_s(m) = Σ_{i=1..m}s(i) ≥ n, k = n – cum_s(m-1) –
 *            1. (Row‑major flattening of the irregular table)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   gcd, mod_pow, pow_int, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/gcd.h"
#include "../utils/pow_int.h"
#include "../utils/mod_pow.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *period = NULL;      /* 1‑based array of s(n) values */
static size_t period_cap = 0;
static size_t period_len = 0;     /* number of rows already computed */

static seq_t *cum = NULL;         /* cumulative sums, cum[0] = 0, 1‑based for rows */
static size_t cum_cap = 0;

/* Ensure period has space for indices up to needed-1 (inclusive). */
static void ensure_period(size_t needed)
{
    if (needed <= period_cap) return;
    size_t newcap = period_cap ? period_cap << 1 : 8;
    while (newcap < needed) newcap <<= 1;
    period = seq_realloc(period, period_cap, newcap);
    period_cap = newcap;
}

/* Ensure cum has space for indices up to needed-1 (inclusive). */
static void ensure_cum(size_t needed)
{
    if (needed <= cum_cap) return;
    size_t newcap = cum_cap ? cum_cap << 1 : 8;
    while (newcap < needed) newcap <<= 1;
    cum = seq_realloc(cum, cum_cap, newcap);
    cum_cap = newcap;
}

/* Carmichael lambda of n via prime factorisation. */
static seq_t lambda_of(idx_t n)
{
    seq_t lam = 1;
    idx_t tmp = n;

    for (idx_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            size_t e = 0;
            while (tmp % p == 0) { tmp /= p; ++e; }

            seq_t lam_p;
            if (p == 2)
            {
                if (e == 1)        lam_p = 1;
                else if (e == 2)   lam_p = 2;
                else                /* e >= 3 */
                    lam_p = pow_int((seq_t)2, (idx_t)(e - 2));
            }
            else
            {
                lam_p = pow_int((seq_t)p, (idx_t)(e - 1));
                lam_p *= ((seq_t)p - 1);
            }

            seq_t g = gcd(lam, lam_p);
            lam = (lam / g) * lam_p;
        }
    }

    if (tmp > 1)      /* remaining prime factor */
    {
        idx_t p = tmp;
        seq_t lam_p = (p == 2) ? 1 : ((seq_t)p - 1);
        seq_t g = gcd(lam, lam_p);
        lam = (lam / g) * lam_p;
    }

    return lam;
}

/* Compute s(n) = lcm(n, λ(n)). */
static seq_t period_of(idx_t n)
{
    seq_t lam = lambda_of(n);
    seq_t g   = gcd((seq_t)n, lam);
    return ((seq_t)n / g) * lam;
}

/* OEIS sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* undefined below offset */

    /* Initialise caches on first call. */
    if (!cum)
    {
        cum_cap = 1;
        cum = seq_alloc(1);
        cum[0] = 0;

        period_cap = 0;
        period = NULL;
        period_len = 0;
    }

    /* Generate rows until the cumulative sum reaches position n. */
    while (cum[period_len] < (seq_t)n)
    {
        idx_t next_row = period_len + 1;
        seq_t s_val = period_of(next_row);

        ensure_period(next_row + 1);
        period[next_row] = s_val;

        ensure_cum(next_row + 1);
        cum[next_row] = cum[period_len] + s_val;

        period_len = next_row;
    }

    /* Locate the row containing the nth term. */
    size_t row = 1;
    while (cum[row] < (seq_t)n) ++row;

    seq_t prev_sum = cum[row - 1];
    idx_t k = (idx_t)((seq_t)n - prev_sum - 1);

    seq_t s_val = period[row];
    seq_t base  = ((seq_t)k) + s_val;
    seq_t mod   = (seq_t)row;

    /* The exponent fits in 64‑bit for the required n range. */
    uint64_t exp = (uint64_t)(base);

    return mod_pow(base, exp, mod);
}
