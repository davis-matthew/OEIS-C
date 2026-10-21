/*
 * A262491  The index of the first of two consecutive positive triangular
 *          numbers (A000217) the sum of which is equal to the sum of eleven
 *          consecutive positive triangular numbers.
 * https://oeis.org/A262491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence * a(n) = 394*a(n-4) -
 *            a(n-8) + 392 (n ≥ 9) * with the first eight terms * a(1)=43,
 *            a(2)=120, a(3)=549, a(4)=3783, * a(5)=17214, a(6)=47629,
 *            a(7)=216688, a(8)=1490884
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* ----- cache (1‑based, stored as 0‑based) ----- */
static seq_t *cache    = NULL;   /* allocated array */
static size_t cap      = 0;      /* allocated capacity */
static size_t sz       = 0;      /* number of terms stored (>=1) */

/* ----- initial terms ----- */
static const seq_t init[8] = {
    (seq_t)43,
    (seq_t)120,
    (seq_t)549,
    (seq_t)3783,
    (seq_t)17214,
    (seq_t)47629,
    (seq_t)216688,
    (seq_t)1490884
};

/* ----- ensure minimum capacity ----- */
static void ensure_capacity(size_t n)
{
    if (cap >= n) return;
    size_t new_cap = (n > 128) ? n * 2 : 128;
    seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!p) abort();              /* memory exhausted */
    cache = p;
    cap   = new_cap;
}

/* ----- compute terms up to index n (1‑based) ----- */
static void ensure_terms(idx_t n)
{
    if (n == 0) return;
    if (sz >= (size_t)n) return;      /* already computed */

    ensure_capacity((size_t)n);

    /* fill from current size to n-1 (0‑based) */
    for (idx_t i = (idx_t)sz; i < n; ++i)
    {
        if ((size_t)i < 8)          /* initial terms */
        {
            cache[i] = init[i];
        }
        else                        /* recurrence */
        {
            /* multiply by 394 safely:
             * for 64‑bit or smaller user types use 128‑bit intermediate,
             * for large arbitrary–precision types just use operator *. */
            seq_t prod;
            if (sizeof(seq_t) <= 8) {
                prod = (seq_t)((unsigned __int128)394 * cache[i-4]);
            } else {
                prod = (seq_t)(394 * cache[i-4]);
            }

            /* prod - cache[i-8] + 392 */
            seq_t val = prod - cache[i-8];
            val = val + (seq_t)392;
            cache[i] = val;
        }
    }
    sz = (size_t)n;
}

/* ----- required external function ----- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* unused, but harmless */
    ensure_terms(n);
    return cache[n-1];
}
