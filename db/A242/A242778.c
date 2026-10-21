/*
 * A242778  Sides (a,c) of cyclic quadrilaterals of integer sides (a,b,c,d),
 *          integer areas, and integer circumradius such that a=b and c=d.
 * https://oeis.org/A242778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth side of a cyclic quadrilateral with integer
 *            sides (a,a,c,c), integer area and integer circumradius. The
 *            sides (a,c) must satisfy a^2 + c^2 = d^2 with d even. Therefore
 *            (a,c) are the legs of a Pythagorean triple multiplied by an even
 *            factor. The sequence is obtained by listing (a,c...
 * Range:     u8 none, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


typedef struct { seq_t a; seq_t c; } pair_t;

/* ------------------------------------------------------------------ */
/*  Local GCD for seq_t                                               */
/* ------------------------------------------------------------------ */
static seq_t seq_gcd_local(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* ------------------------------------------------------------------ */
/*  Global cache of generated pairs                                   */
/* ------------------------------------------------------------------ */
static pair_t *pairs      = NULL;   /* array of pairs, sorted */
static size_t  pairs_cap  = 0;     /* allocated capacity */
static size_t  pairs_cnt  = 0;     /* number of stored pairs  */
static idx_t   max_limit  = 0;     /* maximum ‘a’ value built so far */

/* ------------------------------------------------------------------ */
/*  Comparator for qsort (by a then c)                                */
/* ------------------------------------------------------------------ */
static int cmp_pair(const void *p1, const void *p2)
{
    const pair_t *x = (const pair_t*)p1;
    const pair_t *y = (const pair_t*)p2;
    if (x->a < y->a) return -1;
    if (x->a > y->a) return  1;
    if (x->c < y->c) return -1;
    if (x->c > y->c) return  1;
    return 0;
}

/* ------------------------------------------------------------------ */
/*  Generate all qualifying pairs with smallest side <= limit       */
/* ------------------------------------------------------------------ */
static void regenerate_pairs(idx_t limit)
{
    /* free old data */
    if (pairs) free(pairs);
    pairs     = NULL;
    pairs_cap = 0;
    pairs_cnt = 0;

    /* find maximum m such that m^2 <= limit  (integer sqrt) */
    idx_t m_limit = 0;
    while ((m_limit + 1) * (m_limit + 1) <= limit) ++m_limit;

    for (idx_t m = 2; m <= m_limit; ++m) {
        for (idx_t n = 1; n < m; ++n) {
            if ( ((m - n) & 1) == 0 ) continue;                /* opposite parity  */
            if ( seq_gcd_local((seq_t)m, (seq_t)n) != (seq_t)1 ) continue; /* coprime   */

            /* primitive triple: legs a0,b0 */
            seq_t mm = (seq_t)m;
            seq_t nn = (seq_t)n;
            seq_t a0  = mm * mm - nn * nn;
            seq_t b0  = (seq_t)2 * mm * nn;
            if (a0 > b0) { seq_t tmp = a0; a0 = b0; b0 = tmp; }

            if (a0 > (seq_t)limit) continue;  /* smallest side larger than limit */

            /* only even multipliers give integer circumradius */
            for (idx_t k = 2; ; k += 2) {
                seq_t ak = a0 * k;
                if (ak > (seq_t)limit) break;
                seq_t bk = b0 * k;

                /* enlarge storage if needed */
                if (pairs_cnt == pairs_cap) {
                    size_t new_cap = pairs_cap ? pairs_cap << 1 : 8;
                    pair_t *tmp = (pair_t*)realloc(pairs, new_cap * sizeof(pair_t));
                    if (!tmp) { free(pairs); abort(); }
                    pairs     = tmp;
                    pairs_cap = new_cap;
                }
                pairs[pairs_cnt].a = ak;
                pairs[pairs_cnt].c = bk;
                ++pairs_cnt;
            }
        }
    }

    /* sort pairs by a then c */
    if (pairs_cnt > 1) {
        qsort(pairs, pairs_cnt, sizeof(pair_t), cmp_pair);
    }
}

/* ------------------------------------------------------------------ */
/*  Ensure we have at least “needed” pairs in the cache                */
/* ------------------------------------------------------------------ */
static void ensure_pairs(size_t needed)
{
    if (pairs_cnt >= needed) return;

    /* start with a moderate limit and double until we have enough data */
    idx_t limit = max_limit ? max_limit << 1 : 1000;
    while (1) {
        regenerate_pairs(limit);
        if (pairs_cnt >= needed) {
            max_limit = limit;
            return;
        }
        limit <<= 1;
    }
}

/* ------------------------------------------------------------------ */
/*  The required sequence function                                     */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset is 1 */

    /* pair index (1‑based) = ceil(n/2) */
    size_t pair_index = (size_t)((n + 1) >> 1);
    ensure_pairs(pair_index);

    const pair_t *p = &pairs[pair_index - 1];
    return (n & (idx_t)1) ? p->a : p->c;
}
