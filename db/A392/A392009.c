/*
 * A392009  Even squarefree integers with at least 3 prime factors.
 * https://oeis.org/A392009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 9767, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed via sieving odd squarefree integers with at least 2 distinct prime
   factors; each such odd m gives a term 2*m. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*--- static state --------------------------------------------------------*/
static seq_t *seq_cache = NULL;   /* cache of computed terms                  */
static size_t seq_len   = 0;       /* number of cached terms                  */
static size_t seq_cap   = 0;       /* capacity of seq_cache                   */

static uint64_t last_scanned = 1;   /* last odd number that has been processed */
static uint64_t sieve_limit  = 0;   /* current upper bound for the sieve        */

/*--- helper: grow cache -----------------------------------------------*/
static void grow_cache(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 128;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq_cache = tmp;
    seq_cap   = new_cap;
}

/*--- sieve for smallest prime factor, squarefree flag and prime count ---*/
static void build_sieve(uint64_t limit,
                        uint32_t **spf_out,
                        uint8_t **sqf_out,
                        uint8_t **cnt_out)
{
    size_t sz = (size_t)(limit + 1);
    uint32_t *spf = (uint32_t *)calloc(sz, sizeof(uint32_t));
    uint8_t   *sqf = (uint8_t   *)calloc(sz, sizeof(uint8_t));
    uint8_t   *cnt = (uint8_t   *)calloc(sz, sizeof(uint8_t));
    if (!spf || !sqf || !cnt) abort();

    for (size_t i = 2; i < sz; ++i) spf[i] = (uint32_t)i;

    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (spf[i] == (uint32_t)i) {
            for (uint64_t j = i * i; j <= limit; j += i) {
                if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;
            }
        }
    }

    /* 1 is treated as squarefree for convenience (not used).             */
    sqf[0] = 0; sqf[1] = 1;
    cnt[0] = 0; cnt[1] = 0;

    for (size_t i = 2; i < sz; ++i) {
        uint32_t p = spf[i];
        uint32_t m = (uint32_t)(i / p);
        if (p == spf[m]) {
            /* repeated prime factor ⇒ not squarefree */
            sqf[i] = 0;
            cnt[i] = cnt[m];          /* value irrelevant for non‑squarefree */
        } else {
            sqf[i] = sqf[m];
            cnt[i] = cnt[m] + 1;
        }
    }

    *spf_out = spf;
    *sqf_out = sqf;
    *cnt_out = cnt;
}

/*--- expand cache until at least n terms -----------------------------*/
static void ensure_n_terms(idx_t n)
{
    if ((idx_t)seq_len >= n) return;                     /* already have enough */

    uint64_t lim = (sieve_limit == 0) ? 1000ULL : sieve_limit;

    while ((idx_t)seq_len < n) {
        /* build sieve up to lim */
        uint32_t *spf = NULL;
        uint8_t   *sqf = NULL;
        uint8_t   *cnt = NULL;
        build_sieve(lim, &spf, &sqf, &cnt);

        /* start scanning from the next odd number after last_scanned */
        uint64_t start = last_scanned + 1;
        if ((start & 1) == 0) ++start;     /* ensure start is odd */

        for (uint64_t m = start; m <= lim; m += 2) {
            if (sqf[m] && cnt[m] >= 2) {
                grow_cache(seq_len + 1);
                seq_cache[seq_len++] = (seq_t)m * (seq_t)2;
            }
        }

        last_scanned = lim;                /* m up to lim has been processed */
        sieve_limit  = lim;
        lim *= 2ULL;                       /* next round doubles the bound */

        free(spf);
        free(sqf);
        free(cnt);
    }
}

/*--- required function -----------------------------------------------*/
seq_t a(idx_t n)
{
    ensure_n_terms(n);
    return seq_cache[n - 1];
}
