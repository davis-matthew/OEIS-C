/*
 * A317134  G.f.: Sum_{n>=0} binomial(4*(n+1), n)/(n+1) * x^n /
 *          (1+x)^(2*(n+1)).
 * https://oeis.org/A317134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} (-1)^(n-k) * binom(4*(k+1), k)/(k+1) *
 *            binom(2*(k+1)+(n-k)-1, n-k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 27,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/binomial.h"   /* for big‑integer path only          */

/* Static table of computed values: a_tbl[0 .. a_size-1] = a(m) */
static seq_t *a_tbl   = NULL;
static size_t a_size  = 0;

/* multiplicative binomial using __int128 (safe for the sizes needed) */
static __int128 binom128(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;
    __int128 res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res = res / i;
    }
    return res;
}

/* Ensure a_tbl contains values up to index 'req' */
static void ensure_capacity(idx_t req) {
    if (a_size > (size_t)req) return;              /* already enough */

    /* resize table */
    size_t target = (size_t)req + 1;
    if (target < 8) target = 8;
    seq_t *tmp = (seq_t *)realloc(a_tbl, target * sizeof(seq_t));
    if (!tmp) return;  /* allocation failed – caller will get garbage */
    a_tbl = tmp;

    /* initialize new slots */
    for (size_t i = a_size; i < target; ++i) a_tbl[i] = (seq_t)0;

    /* first term */
    if (a_size == 0) {
        a_tbl[0] = (seq_t)1;
        a_size = 1;
    }

    /* compute new terms */
    for (idx_t n = a_size; n <= req; ++n) {
        /* use 128‑bit arithmetic if seq_t is small; else use big‑int ops */
        if (sizeof(seq_t) <= 8) {
            __int128 pos = 0, neg = 0;
            for (idx_t k = 0; k <= n; ++k) {
                __int128 u = binom128(4 * (k + 1), k) / (k + 1);
                __int128 v = binom128(2 * (k + 1) + (n - k) - 1, n - k);
                __int128 term = u * v;
                if ((n - k) & 1) neg += term;
                else            pos += term;
            }
            a_tbl[n] = (seq_t)(pos - neg);
        } else {
            seq_t pos = 0, neg = 0;
            for (idx_t k = 0; k <= n; ++k) {
                seq_t u  = binomial(4 * (k + 1), k);
                u /= (k + 1);                       /* exact division */
                seq_t v  = binomial(2 * (k + 1) + (n - k) - 1, n - k);
                seq_t term = u * v;
                if ((n - k) & 1) neg += term;
                else            pos += term;
            }
            a_tbl[n] = pos - neg;
        }
    }
    a_size = (size_t)req + 1;
}

/* Public sequence function */
seq_t a(idx_t n) {
    ensure_capacity(n);
    return a_tbl[n];
}
