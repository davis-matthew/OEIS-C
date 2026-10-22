/*
 * A306268  Number of ways to choose a strict factorization into squarefree
 *          factors of each factor in a strict factorization of n.
 * https://oeis.org/A306268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{n = f1*…*fk, 1<f1<…<fk} ∏_{i=1..k} g(fi) where g(m) is
 *            the number of strict factorizations of m into squarefree
 *            factors. a(1) = 1. All other values are computed recursively
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>


static idx_t floor_sqrt(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= n)
        ++r;
    return r;
}

static bool is_squarefree(idx_t n)
{
    if (n <= 1)
        return true;
    for (idx_t p = 2; p * p <= n; ++p)
    {
        if (n % p == 0)
        {
            n /= p;
            if (n % p == 0)
                return false;
            while (n % p == 0)
                n /= p;
        }
    }
    return true;
}

/* ----------  Memoisation for g(n)  ---------- */
static seq_t *g_cache = NULL;
static bool  *g_done  = NULL;
static idx_t   g_cap   = 0;

static void ensure_g(idx_t need)
{
    if (need < g_cap)
        return;
    idx_t new_cap = (need + 1) * 2;
    g_cache = (seq_t *)realloc(g_cache, new_cap * sizeof(seq_t));
    g_done  = (bool  *)realloc(g_done , new_cap * sizeof(bool));
    for (idx_t i = g_cap; i < new_cap; ++i)
        g_done[i] = false;
    g_cap = new_cap;
}

static inline seq_t g_of(idx_t n);

static seq_t g_rec(idx_t n, idx_t min_factor)
{
    if (n < min_factor)
        return 0;
    seq_t cnt = 0;
    if (is_squarefree(n))
        cnt += 1;                         /* trivial factorisation [n] */
    idx_t lim = floor_sqrt(n);
    for (idx_t d = min_factor; d <= lim; ++d)
    {
        if (n % d == 0)
        {
            idx_t r = n / d;
            if (d < r && is_squarefree(d))
                cnt += g_rec(r, d + 1);
        }
    }
    return cnt;
}

static inline seq_t g_of(idx_t n)
{
    ensure_g(n);
    if (g_done[n])
        return g_cache[n];
    g_done[n] = true;
    g_cache[n] = g_rec(n, 2);
    return g_cache[n];
}

/* ----------  Memoisation for a(n)  ---------- */
static seq_t *a_cache = NULL;
static bool  *a_done  = NULL;
static idx_t   a_cap   = 0;

static void ensure_a(idx_t need)
{
    if (need < a_cap)
        return;
    idx_t new_cap = (need + 1) * 2;
    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    a_done  = (bool  *)realloc(a_done , new_cap * sizeof(bool));
    for (idx_t i = a_cap; i < new_cap; ++i)
        a_done[i] = false;
    a_cap = new_cap;
}

static seq_t a_rec(idx_t n, idx_t min_factor)
{
    if (n < min_factor)
        return 0;
    seq_t sum = 0;
    /* single‑group factorisation */
    sum += g_of(n);
    idx_t lim = floor_sqrt(n);
    for (idx_t d = min_factor; d <= lim; ++d)
    {
        if (n % d == 0)
        {
            idx_t r = n / d;
            if (d < r)
                sum += g_of(d) * a_rec(r, d + 1);
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    ensure_a(n);
    if (a_done[n])
        return a_cache[n];
    a_done[n] = true;
    a_cache[n] = a_rec(n, 2);
    return a_cache[n];
}
