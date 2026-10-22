/*
 * A379771  Number of k <= n that are neither squarefree nor prime powers.
 * https://oeis.org/A379771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #k ≤ n that are neither squarefree nor a prime power. * *
 *            Implementation: Brute‑force sieve of smallest‑prime‑factor (spf)
 *            up to the * current n and a cumulative count array. The spf
 *            array is rebuilt only * when a larger bound is required. The
 *            cumulative array grows * logarithmically
 * Range:     u8 n <= 715, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* ---- local helper – build a smallest‑prime‑factor sieve ---- */
static void _build_spf(idx_t n, idx_t **spf_out)
{
    /* Allocate array [0..n] and set default spf[i] = i. */
    idx_t *spf = (idx_t*)malloc((n + 1) * sizeof(idx_t));
    if (!spf) exit(EXIT_FAILURE);
    for (idx_t i = 0; i <= n; ++i) spf[i] = i;

    for (idx_t i = 2; i * i <= n; ++i)
    {
        if (spf[i] != i) continue;          /* i is not prime */
        for (idx_t j = i * i; j <= n; j += i)
            if (spf[j] == j) spf[j] = i;     /* set smallest prime factor */
    }
    *spf_out = spf;
}

/* ---- global state for sieve and cumulative array ---- */
static idx_t *spf = NULL;          /* smallest prime factor array, 1‑based */
static idx_t spf_limit = 0;        /* highest index spf currently covers */

static seq_t *cum = NULL;          /* cumulative counts: cum[0] = 0 */
static size_t cum_cap = 0;         /* allocated capacity of cum */
static idx_t cum_len = 0;          /* largest index already computed in cum */

/* ---- ensure spf covers up to 'n' ---- */
static void ensure_spf(idx_t n)
{
    if (n <= spf_limit) return;
    if (spf) free(spf);
    _build_spf(n, &spf);
    spf_limit = n;
}

/* ---- make sure cumulative array can hold at least 'needed' elements ---- */
static void ensure_cum_capacity(idx_t needed)
{
    if (needed + 1 <= cum_cap) return;   /* include index 0 */
    size_t new_cap = cum_cap ? cum_cap : 16;
    while (new_cap < (size_t)(needed + 1)) new_cap <<= 1;
    seq_t *new_arr = (seq_t*)realloc(cum, new_cap * sizeof(seq_t));
    if (!new_arr) exit(EXIT_FAILURE);
    cum = new_arr;
    cum_cap = new_cap;
}

/* ---- extend cumulative count array up to n ---- */
static void ensure_cum(idx_t n)
{
    if (cum_len >= n) return;
    ensure_spf(n);
    ensure_cum_capacity(n);

    for (idx_t i = cum_len + 1; i <= n; ++i)
    {
        bool is_target = false;
        if (i >= 4)          /* smallest possible target is 12 */
        {
            idx_t x = i;
            int distinct = 0;
            bool has_square = false;
            while (x > 1)
            {
                idx_t p = spf[x];
                int exp = 0;
                do
                {
                    x /= p;
                    ++exp;
                } while (x % p == 0);
                if (exp >= 2) has_square = true;
                ++distinct;
                if (distinct > 1 && has_square)
                {
                    is_target = true;
                    break;
                }
            }
        }
        cum[i] = cum[i - 1] + (seq_t)(is_target);
    }
    cum_len = n;
}

/* ---- required public function ---- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* out of bounds – harmless */
    ensure_cum(n);
    return cum[n];
}
