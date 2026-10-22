/*
 * A336497  Numbers that cannot be written as a product of superfactorials
 *          A000178.
 * https://oeis.org/A336497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of A336497: numbers that cannot be expressed
   as a product of superfactorials (OEIS A000178).  The function builds
   a cache of representable numbers up to a dynamic limit and returns
   the nth smallest non‑representable number. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* static data used by repeated calls */
static seq_t *nonrep_cache = NULL;    /* array of nonrepresentable numbers   */
static size_t nonrep_size = 0;        /* number of elements in the array     */
static idx_t  current_limit = 0;      /* last limit used for building the cache */

/* build the cache for a given numeric limit */
static void build_limit(idx_t limit)
{
    /* free previous cache */
    if (nonrep_cache) {
        free(nonrep_cache);
        nonrep_cache = NULL;
    }

    /* ----- compute superfactorials <= limit ----- */
    uint64_t *sfs = NULL;
    size_t   sf_cnt = 0, sf_cap = 8;
    sfs = (uint64_t *)malloc(sf_cap * sizeof(uint64_t));
    if (!sfs) exit(EXIT_FAILURE);

    uint64_t fact = 1, sf_prod = 1;
    for (uint64_t k = 2; ; ++k) {
        fact   *= k;
        sf_prod*= fact;
        if (sf_prod > limit) break;
        if (sf_cnt == sf_cap) {
            sf_cap <<= 1;
            uint64_t *tmp = (uint64_t *)realloc(sfs, sf_cap * sizeof(uint64_t));
            if (!tmp) exit(EXIT_FAILURE);
            sfs = tmp;
        }
        sfs[sf_cnt++] = sf_prod;
    }

    /* ----- visited array for representable numbers ----- */
    size_t lim = (size_t)limit;
    bool *vis = (bool *)calloc(lim + 1, sizeof(bool));
    if (!vis) exit(EXIT_FAILURE);
    vis[1] = true;                /* 1 is representable (empty product) */

    /* ----- queue for breadth‑first traversal ----- */
    size_t qcap = 1024;
    uint64_t *queue = (uint64_t *)malloc(qcap * sizeof(uint64_t));
    if (!queue) exit(EXIT_FAILURE);
    size_t qhead = 0, qtail = 0; 
    queue[qtail++] = 1;           /* start with 1 */

    while (qhead < qtail) {
        uint64_t cur = queue[qhead++];
        for (size_t i = 0; i < sf_cnt; ++i) {
            uint64_t sf = sfs[i];
            if (cur > lim / sf) continue;   /* would overflow limit */
            uint64_t nxt = cur * sf;
            size_t idx = (size_t)nxt;
            if (!vis[idx]) {
                vis[idx] = true;
                if (qtail == qcap) {
                    qcap <<= 1;
                    uint64_t *tmp = (uint64_t *)realloc(queue, qcap * sizeof(uint64_t));
                    if (!tmp) exit(EXIT_FAILURE);
                    queue = tmp;
                }
                queue[qtail++] = nxt;
            }
        }
    }

    /* ----- collect non‑representable numbers ----- */
    size_t cap = 1024;
    seq_t *nr = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!nr) exit(EXIT_FAILURE);
    size_t sz = 0;
    for (idx_t v = 2; v <= limit; ++v) {
        if (!vis[(size_t)v]) {
            if (sz == cap) {
                cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(nr, cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                nr = tmp;
            }
            nr[sz++] = (seq_t)v;
        }
    }

    /* free temporaries */
    free(vis);
    free(queue);
    free(sfs);

    /* store results */
    nonrep_cache = nr;
    nonrep_size  = sz;
    current_limit = limit;
}

/* public function requested */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* outside the defined range */

    /* initialise cache if first call */
    if (!nonrep_cache) build_limit(1000000);

    /* enlarge limit until we have enough terms */
    while (n > nonrep_size) {
        build_limit(current_limit * 2 + 1);
    }

    return nonrep_cache[n - 1];
}
