/*
 * A355121  E.g.f. A(x) satisfies A(x) = 1 - log(1-x) * A(-2 * log(1-x)).
 * https://oeis.org/A355121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A355121
   e.g.f. A(x) satisfies: A(x) = 1 - log(1-x) * A(-2 * log(1-x)).
   Recurrence: a(0)=1; a(n)=∑_{k=1..n} k·2^{k-1}·|Stirling1(n,k)|·a(k-1)  (n≥1)  */
/* external types supplied by the build */

#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------ */
/*  Memoised cache for a(n) and auxiliary tables                */
/* ------------------------------------------------------------ */
static seq_t *a_cache = NULL;     /* computed a(n) values          */
static seq_t *st_tbl  = NULL;     /* unsigned Stirling numbers of 1st kind: |s(n,k)|   */
static seq_t *pow2_tbl = NULL;    /* powers of 2: 2^k                */
static idx_t  a_cap   = 0;
static idx_t  st_cap  = 0;
static idx_t  pow2_cap= 0;
static idx_t  last_n  = UINT64_MAX;  /* sentinel: nothing cached yet */

/* ------------------------------------------------------------ */
/*  Ensure a backing array has at least 'need' elements          */
/* ------------------------------------------------------------ */
static void ensure_cap(idx_t *cap, seq_t **arr, idx_t need)
{
    if (need <= *cap) return;
    idx_t new_cap = (*cap) ? (*cap) : 1;
    while (new_cap < need) new_cap <<= 1;
    *arr = (seq_t*)realloc(*arr, new_cap * sizeof(seq_t));
    /* zero new entries */
    for (idx_t i = *cap; i < new_cap; ++i) (*arr)[i] = (seq_t)0;
    *cap = new_cap;
}

/* ------------------------------------------------------------ */
/*  Compute a(n)  (any order of calls)                         */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* initialise tables on first call */
    if (a_cache == NULL) {
        /* a(0) = 1 */
        a_cap = 1;
        a_cache = (seq_t*)malloc(sizeof(seq_t));
        a_cache[0] = (seq_t)1;
        last_n = 0;

        st_cap = 1;
        st_tbl = (seq_t*)malloc(sizeof(seq_t));
        st_tbl[0] = (seq_t)1;     /* |s(0,0)| = 1 */

        pow2_cap = 1;
        pow2_tbl = (seq_t*)malloc(sizeof(seq_t));
        pow2_tbl[0] = (seq_t)1;   /* 2^0 = 1 */
    }

    /* if requested n already computed, return it */
    if (n <= last_n) return a_cache[n];

    /* extend tables to accommodate n */
    ensure_cap(&a_cap,    &a_cache,    n + 1);
    ensure_cap(&st_cap,   &st_tbl,     n + 1);
    ensure_cap(&pow2_cap, &pow2_tbl,   n + 1);

    /* compute terms from last_n+1 up to n */
    for (idx_t cur = last_n + 1; cur <= n; ++cur) {
        /* ---- update powers of two ---- */
        if (cur > 0)
            pow2_tbl[cur] = pow2_tbl[cur-1] * (seq_t)2;   /* 2^cur */

        /* ---- update Stirling numbers for row cur (unsigned) ---- */
        /* make a space for new element k=cur */
        st_tbl[cur] = (seq_t)0;
        for (idx_t k = cur; k >= 1; --k) {
            seq_t left  = st_tbl[k-1];              /* |s(cur-1,k-1)| from previous row */
            seq_t right = (seq_t)(cur-1) * st_tbl[k]; /* (cur-1)*|s(cur-1,k)| */
            st_tbl[k] = left + right;                /* |s(cur,k)| */
        }
        st_tbl[0] = (seq_t)0;   /* |s(cur,0)| = 0 for cur>0 */

        /* ---- compute a[cur] via the recurrence ---- */
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= cur; ++k) {
            seq_t factor  = (seq_t)k * pow2_tbl[k-1];      /* k·2^{k-1} */
            seq_t prod    = a_cache[k-1] * st_tbl[k];      /* a(k-1)·|s(cur,k)| */
            sum += factor * prod;
        }
        a_cache[cur] = sum;
    }

    last_n = n;
    return a_cache[n];
}
