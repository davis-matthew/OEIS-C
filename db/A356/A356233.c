/*
 * A356233  Number of integer factorizations of n into gapless numbers
 *          (A066311).
 * https://oeis.org/A356233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered factorizations of n into gapless
 *            factors. (Gapless: the prime indices of the distinct prime
 *            factors of a number form a contiguous interval.)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Global data cached between calls */
static idx_t *g_spf = NULL;                  /* smallest prime factor table */
static uint32_t *g_prime_index = NULL;       /* prime index table */
static uint8_t *g_is_gap = NULL;             /* gapless flag table */
static struct DivList {
    idx_t *divs;     /* list of divisors of n (ascending) */
    size_t len;      /* number of divisors */
} *g_dlist = NULL;                         /* divisor lists for all n */
static idx_t g_cur_limit = 0;               /* largest n for which tables are built */

static void rebuild_tables(idx_t limit)
{
    /* free tables of previous size */
    if (g_spf) free(g_spf);
    if (g_prime_index) free(g_prime_index);
    if (g_is_gap) free(g_is_gap);
    if (g_dlist) {
        for (idx_t i = 1; i <= g_cur_limit; ++i)
            if (g_dlist[i].divs) free(g_dlist[i].divs);
        free(g_dlist);
    }

    /* allocate new tables */
    size_t sz = (size_t)limit + 1;
    g_spf = (idx_t*)calloc(sz, sizeof(idx_t));
    g_prime_index = (uint32_t*)calloc(sz, sizeof(uint32_t));
    g_is_gap = (uint8_t*)calloc(sz, sizeof(uint8_t));
    g_dlist = (struct DivList*)calloc(sz, sizeof(struct DivList));

    /* sieve: smallest prime factor */
    for (idx_t i = 0; i <= limit; ++i) g_spf[i] = i;
    for (idx_t i = 2; i * i <= limit; ++i)
        if (g_spf[i] == i)
            for (idx_t j = i * i; j <= limit; j += i)
                if (g_spf[j] == j) g_spf[j] = i;
    g_spf[0] = 0; g_spf[1] = 1;

    /* prime indices */
    uint32_t prime_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (g_spf[i] == i) {
            ++prime_cnt;
            g_prime_index[i] = prime_cnt;      /* indices start at 1 */
        }

    /* gapless flag table */
    g_is_gap[1] = 1;                            /* convention: 1 is gapless */
    for (idx_t n = 2; n <= limit; ++n) {
        idx_t m = n;
        int min_idx = 0x7fffffff;
        int max_idx = 0;
        int distinct = 0;
        while (m > 1) {
            idx_t p = g_spf[m];
            int idx = (int)g_prime_index[p];
            if (idx < min_idx) min_idx = idx;
            if (idx > max_idx) max_idx = idx;
            ++distinct;
            while (m % p == 0) m /= p;
        }
        g_is_gap[n] = (max_idx - min_idx + 1 == distinct) ? 1 : 0;
    }

    /* divisor lists */
    size_t *divcnt = (size_t*)calloc(sz, sizeof(size_t));
    for (idx_t i = 1; i <= limit; ++i)
        for (idx_t j = i; j <= limit; j += i)
            ++divcnt[j];
    for (idx_t i = 1; i <= limit; ++i) {
        g_dlist[i].len = divcnt[i];
        g_dlist[i].divs = (idx_t*)malloc(divcnt[i] * sizeof(idx_t));
    }
    free(divcnt);

    size_t *pos = (size_t*)calloc(sz, sizeof(size_t));
    for (idx_t i = 1; i <= limit; ++i)
        for (idx_t j = i; j <= limit; j += i) {
            size_t k = pos[j]++;
            g_dlist[j].divs[k] = i;
        }
    free(pos);

    g_cur_limit = limit;
}

/* Recursive count of factorizations */
static seq_t count_part(idx_t n, idx_t min_factor)
{
    if (n == 1) return (seq_t)1;          /* empty product counts as one */
    seq_t res = 0;
    const struct DivList *dl = &g_dlist[n];
    for (size_t i = 0; i < dl->len; ++i) {
        idx_t d = dl->divs[i];
        if (d == 1 || d < min_factor) continue; /* skip 1 and non‑decreasing rule */
        if (!g_is_gap[d]) continue;              /* only gapless factors */
        res += count_part(n / d, d);
    }
    return res;
}

/* Public function required by the specification */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* safety for misuse */
    if (g_cur_limit < n) rebuild_tables(n);
    return count_part(n, (idx_t)1);
}
