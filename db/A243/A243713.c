/*
 * A243713  Irregular triangular array of numerators of all positive rational
 *          numbers ordered as in Comments.
 * https://oeis.org/A243713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf, frac
 * Formula:   Generate the irregular triangular array of all positive
 *            rationals * as described in OEIS A243713. Row 1 = {1/1}. For n ≥
 *            4, * row n consists of 1+x for each fraction in row n‑1,
 *            together with * x/(x+1) for each fraction in row n‑3, sorted in
 *            decreasing order. * The sequence a(n) is the numerator...
 * Range:     u8 n <= 133, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* Local GCD routine – works for any unsigned seq_t type.                */
static seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != 0u) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* --------------------------------------------------------------------- */
/* Fraction representation used while generating rows.                  */
typedef struct { seq_t num; seq_t den; } frac_t;

/* Compare two fractions by decreasing value: a/b > c/d  ↔  a*d > c*b. */
static int cmp_frac_desc(const void *A, const void *B)
{
    const frac_t *a = (const frac_t *)A;
    const frac_t *b = (const frac_t *)B;
    /* cross‑multiply – overflow is fine for arbitrary‑precision seq_t */
    seq_t left  = a->num * b->den;
    seq_t right = b->num * a->den;
    if (left > right) return -1;
    if (left < right) return 1;
    return 0;
}

/* --------------------------------------------------------------------- */
/* Cache of numerators produced so far.                                   */
static seq_t *cache      = NULL;   /* cache[n-1] = a(n)                */
static idx_t  cache_size = 0;      /* number of terms stored in cache   */
static size_t cache_cap  = 0;      /* allocated capacity                */

/* Helper to ensure capacity of the cache. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size) return;
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 64;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!p) abort();              /* allocation failure */
        cache = p;
        cache_cap = new_cap;
    }
}

/* --------------------------------------------------------------------- */
/* Three cyclic rows of fractions.                                      */
static frac_t **row_arr = NULL;   /* row_arr[0..2] points to current rows */
static size_t  *row_len = NULL;   /* row_len[0..2] are the row lengths      */
static idx_t   cur_row   = 0;     /* number of the last row produced       */

/* One‑time initialisation: first three rows and cache. */
static void init_once(void)
{
    /* Initialise cache. */
    cache_cap = 64;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) abort();
    cache_size = 0;

    /* Allocate row structures. */
    row_arr = (frac_t **)malloc(3 * sizeof(frac_t *));
    row_len = (size_t *)malloc(3 * sizeof(size_t));
    if (!row_arr || !row_len) abort();

    /* Row 1: 1/1 */
    row_arr[0] = (frac_t *)malloc(sizeof(frac_t));
    row_arr[0][0].num = (seq_t)1;
    row_arr[0][0].den = (seq_t)1;
    row_len[0] = 1;
    cache[cache_size++] = 1;

    /* Row 2: 2/1 (1 + 1/1) */
    row_arr[1] = (frac_t *)malloc(sizeof(frac_t));
    row_arr[1][0].num = (seq_t)2;
    row_arr[1][0].den = (seq_t)1;
    row_len[1] = 1;
    cache[cache_size++] = 2;

    /* Row 3: 3/1 (1 + 2/1) */
    row_arr[2] = (frac_t *)malloc(sizeof(frac_t));
    row_arr[2][0].num = (seq_t)3;
    row_arr[2][0].den = (seq_t)1;
    row_len[2] = 1;
    cache[cache_size++] = 3;

    cur_row = 3;   /* last produced row number */
}

/* --------------------------------------------------------------------- */
/* Main sequence function. */
seq_t a(idx_t n)
{
    static bool first = true;
    if (first) { init_once(); first = false; }

    if (n == 0) return 0;  /* undefined, safe guard */

    /* Generate rows until cache has at least n terms. */
    while (cache_size < n) {
        size_t idx_prev     = ((size_t)cur_row     - 1) % 3;
        size_t idx_prevprev = ((size_t)cur_row - 3) % 3;

        const frac_t *prev      = row_arr[idx_prev];
        size_t        len_prev  = row_len[idx_prev];
        const frac_t *prevprev  = row_arr[idx_prevprev];
        size_t        len_prevprev = row_len[idx_prevprev];

        size_t new_len = len_prev + len_prevprev;
        frac_t *new_row = (frac_t *)malloc(new_len * sizeof(frac_t));
        if (!new_row) abort();

        size_t pos = 0;

        /* 1 + x for each x in row n‑1 */
        for (size_t i = 0; i < len_prev; ++i) {
            seq_t num = prev[i].num + prev[i].den;
            seq_t den = prev[i].den;
            seq_t g   = gcd_seq(num, den);
            num /= g;
            den /= g;
            new_row[pos++] = (frac_t){num, den};
        }

        /* x/(x+1) for each x in row n‑3 */
        for (size_t i = 0; i < len_prevprev; ++i) {
            seq_t num = prevprev[i].num;
            seq_t den = prevprev[i].num + prevprev[i].den;
            seq_t g   = gcd_seq(num, den);
            num /= g;
            den /= g;
            new_row[pos++] = (frac_t){num, den};
        }

        /* Sort the new row in decreasing order. */
        qsort(new_row, new_len, sizeof(frac_t), cmp_frac_desc);

        /* Append numerators to the cache. */
        ensure_cache(cache_size + new_len);
        for (size_t i = 0; i < new_len; ++i)
            cache[cache_size++] = new_row[i].num;

        /* Replace the oldest stored row. */
        size_t replace_idx = (size_t)cur_row % 3;
        free(row_arr[replace_idx]);

        row_arr[replace_idx] = new_row;
        row_len[replace_idx] = new_len;

        ++cur_row;  /* advance to next row number */
    }

    return cache[n - 1];
}
