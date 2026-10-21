/*
 * A288233  Coefficients in the expansion of 1/([r]-[2*r]*x+[3*r]*x^2-...); [
 *          ]=floor, r=sqrt(8/3).
 * https://oeis.org/A288233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A288233
 *  Sequence defined by the recurrence
 *
 *       a(0) = 1
 *       a(n) = Σ_{k=1..n} (-1)^{k+1} * f(k) * a(n-k)   (n ≥ 1)
 *       with   f(k) = ⌊(k+1) * √(8/3)⌋
 *
 *  The function below computes a(n) exactly for any n ≥ 0.
 *  It memoises all computed values so that calls may come in any order.
 *  The implementation uses 128‑bit integers for 64‑bit (and smaller)
 *  seq_t types to avoid overflow of intermediate products.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- floor((k+1)*sqrt(8/3)) as an integer ---------- */
static inline seq_t compute_f(idx_t k)
{
    /* we need the largest v with 3*v^2 ≤ 8*(k+1)^2 */
    unsigned __int128 kk = (unsigned __int128)(k + 1);
    unsigned __int128 m  = (unsigned __int128)8 * kk * kk;

    unsigned long long low = 0;
    unsigned long long high = (unsigned long long)((k + 1) * 2 + 1); /* loose upper bound */

    while (low <= high) {
        unsigned long long mid = low + ((high - low) >> 1);
        unsigned __int128 t = (unsigned __int128)mid * mid * 3;
        if (t <= m) {
            low = mid + 1;
        } else {
            if (mid == 0) break;   /* protection against underflow */
            high = mid - 1;
        }
    }
    return (seq_t)high;
}

/* ---------- memoisation buffers ---------- */
static seq_t *a_cache   = NULL;   /* computed a(n) */
static seq_t *f_cache   = NULL;   /* computed f(k) */
static size_t  a_size   = 0;      /* allocated size of a_cache  */
static size_t  f_size   = 0;      /* allocated size of f_cache  */
static size_t  a_len    = 0;      /* number of entries actually filled in a_cache */
static size_t  f_len    = 0;      /* number of entries actually filled in f_cache */

/* ---------- ensure buffers are large enough ---------- */
static void ensure_capacity(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (a_size < need) {
        seq_t *tmp = (seq_t *)realloc(a_cache, need * sizeof(seq_t));
        if (!tmp) abort();
        a_cache = tmp;
        a_size  = need;
    }
    if (f_size < need) {
        seq_t *tmp = (seq_t *)realloc(f_cache, need * sizeof(seq_t));
        if (!tmp) abort();
        f_cache = tmp;
        f_size  = need;
    }
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* initialise for n==0 on first use */
        if (a_len == 0) {
            ensure_capacity(0);
            a_cache[0] = (seq_t)1;
            f_cache[0] = compute_f(0);
            a_len = 1;
            f_len = 1;
        }
        return a_cache[0];
    }

    ensure_capacity(n);

    /* compute f(k) for all k that are still missing */
    if (f_len <= (size_t)n) {
        for (size_t i = f_len; i <= (size_t)n; ++i)
            f_cache[i] = compute_f((idx_t)i);
        f_len = (size_t)n + 1;
    }

    /* compute a(k) for all k that are still missing */
    if (a_len <= (size_t)n) {
        for (size_t i = a_len; i <= (size_t)n; ++i) {
            if (sizeof(seq_t) <= 8) {
                /* 64‑bit or smaller: use 128‑bit accumulation */
                __int128 pos = 0;
                __int128 neg = 0;
                for (size_t k = 1; k <= i; ++k) {
                    __int128 prod = (__int128)f_cache[k] * (__int128)a_cache[i - k];
                    if (k & 1) pos += prod;   /* k odd -> + */
                    else      neg += prod;   /* k even -> - */
                }
                a_cache[i] = (seq_t)(pos - neg);
            } else {
                /* larger seq_t or arbitrary‑precision: use seq_t arithmetic */
                seq_t pos = 0;
                seq_t neg = 0;
                for (size_t k = 1; k <= i; ++k) {
                    seq_t prod = f_cache[k] * a_cache[i - k];
                    if (k & 1) pos += prod;   /* k odd -> + */
                    else      neg += prod;   /* k even -> - */
                }
                a_cache[i] = pos - neg;
            }
        }
        a_len = (size_t)n + 1;
    }

    return a_cache[n];
}
