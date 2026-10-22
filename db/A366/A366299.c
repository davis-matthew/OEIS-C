/*
 * A366299  Expansion of e.g.f. 1 / (-3 + Sum_{k=1..4} exp(-k*x)).
 * https://oeis.org/A366299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(k+1) * binomial(n,k) * (1 + 2^k + 3^k
 *            + 4^k) * a(n-k); a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes OEIS A366299.  The function is memoised and works for any
    sequence value type seq_t and index type idx_t supplied by the
    build.  No external libraries beyond the standard C headers are
    required.  */

#include <stdlib.h>

/* ------------------------------------------------------------------ */
/*  Cache of previously computed a(n) values                          */
/* ------------------------------------------------------------------ */
static seq_t *a_cache = NULL;   /* a_cache[0..] */
static idx_t  a_cap   = 0;      /* number of slots allocated */

/* ------------------------------------------------------------------ */
/*  Cache of the values c(k) = 1 + 2^k + 3^k + 4^k                    */
/* ------------------------------------------------------------------ */
static seq_t *c_cache = NULL;   /* c_cache[0..] (c_cache[0] unused) */
static idx_t  c_cap   = 0;      /* size of c_cache */

static seq_t pow2 = 1, pow3 = 1, pow4 = 1;   /* powers of 2, 3, 4 for next k */

/* ------------------------------------------------------------------ */
/*  Ensure that a_cache contains entries up to index n                 */
/* ------------------------------------------------------------------ */
static void ensure_a_cache(idx_t n)
{
    if (a_cap <= n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0) exit(1);          /* allocation failure */

        a_cache = tmp;
        if (a_cap == 0) {                         /* initialise a(0) */
            a_cache[0] = 1;                       /* a(0) = 1 */
            a_cap = 1;
        }

        for (idx_t i = a_cap; i <= n; ++i) {
            seq_t sum = 0;
            seq_t binom = 0;                      /* C(i,0) = 1, but we start at k=1 */

            for (idx_t k = 1; k <= i; ++k) {
                if (k == 1)          /* first step: C(i,1) = i */
                    binom = (seq_t) i;
                else
                    binom = (binom * (seq_t)(i - k + 1)) / (seq_t)k;

                seq_t term = binom * c_cache[k] * a_cache[i - k];
                if (k & 1)   /* odd k → plus */
                    sum += term;
                else        /* even k → minus */
                    sum -= term;
            }
            a_cache[i] = sum;
        }
        a_cap = new_cap;
    }
}

/* ------------------------------------------------------------------ */
/*  Ensure that c_cache contains entries up to index n                  */
/* ------------------------------------------------------------------ */
static void ensure_c_cache(idx_t n)
{
    if (c_cap <= n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(c_cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0) exit(1);          /* allocation failure */

        c_cache = tmp;
        if (c_cap == 0) {                         /* initialise powers */
            pow2 = 1;
            pow3 = 1;
            pow4 = 1;
            c_cache[0] = 0;                       /* unused */
            c_cap = 1;
        }

        for (idx_t k = c_cap; k <= n; ++k) {
            pow2 = pow2 * 2;
            pow3 = pow3 * 3;
            pow4 = pow4 * 4;
            c_cache[k] = 1 + pow2 + pow3 + pow4;
        }
        c_cap = new_cap;
    }
}

/* ------------------------------------------------------------------ */
/*  Public function implementing the recurrence                       */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) {                     /* a(0) is defined explicitly */
        return 1;
    }

    ensure_c_cache(n);                 /* need c(k) up to k = n   */
    ensure_a_cache(n);                 /* compute a(n) if not present   */

    return a_cache[n];
}
