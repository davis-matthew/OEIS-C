/*
 * A299543  Solution a( ) of the complementary equation a(n) = 2*b(n-1) +
 *          b(n-2) - b(n-3), where a(0) = 1, a(1) = 2, a(2) = 3; see Comments.
 * https://oeis.org/A299543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A299543
    a(0) = 1, a(1) = 2, a(2) = 3
    for n ≥ 3: a(n) = 2*b(n‑1) + b(n‑2) – b(n‑3)
    where
        b(0) = least positive integer not in {a0,a1,a2}
        b(n) = least positive integer not in {a0,…,an, b0,…,b(n‑1)} for n > 0 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* caches for the two sequences */
static seq_t *a_vals   = NULL;
static idx_t  a_len    = 0;
static idx_t  a_cap    = 0;

static seq_t *b_vals   = NULL;
static idx_t  b_len    = 0;
static idx_t  b_cap    = 0;

/* used marks values that already appeared in either a or b.
   used[v] == 1 means v has been used. */
static unsigned char *used    = NULL;
static idx_t           used_cap = 0;
static idx_t           mex      = 1;   /* smallest excluded positive integer */

/* --- helpers for reallocating memory ---------------------------------- */
static void grow_used(idx_t need)
{
    if (used_cap >= need) return;
    idx_t new_cap = used_cap ? used_cap << 1 : 16;
    if (new_cap < need) new_cap = need;
#ifdef __cplusplus
    unsigned char *tmp = new unsigned char[new_cap + 1U];
    memset(tmp, 0, new_cap + 1U);
    if (used) {
        memcpy(tmp, used, used_cap + 1U);
        delete[] used;
    }
#else
    unsigned char *tmp = (unsigned char *)calloc((size_t)new_cap + 1U, 1);
    if (!tmp) abort();               /* allocation failure */
    if (used) {
        memcpy(tmp, used, (size_t)used_cap + 1U);
        free(used);
    }
#endif
    used      = tmp;
    used_cap  = new_cap;
}

static void mark_used(idx_t v)
{
    grow_used(v);
    used[v] = 1;
    while (mex <= used_cap && used[mex]) ++mex;
}

/* --- initialise caches ------------------------------------------------- */
static void init_cache(idx_t need)
{
    idx_t init_cap = need > 64 ? need : 64;
    a_cap = b_cap = init_cap;

#ifdef __cplusplus
    a_vals = new seq_t[a_cap];
    b_vals = new seq_t[b_cap];
#else
    a_vals = (seq_t *)malloc((size_t)a_cap * sizeof(seq_t));
    b_vals = (seq_t *)malloc((size_t)b_cap * sizeof(seq_t));
#endif
    if (!a_vals || !b_vals) abort();

    /* initial a-values */
    a_vals[0] = (seq_t)1;
    a_vals[1] = (seq_t)2;
    a_vals[2] = (seq_t)3;
    a_len    = 3;

    /* initialise used set with a-values */
    grow_used(10);          /* small starting size */
    mark_used((idx_t)1);
    mark_used((idx_t)2);
    mark_used((idx_t)3);

    /* build first three b-values */
    b_vals[0] = (seq_t)mex;          /* 4 */
    mark_used((idx_t)mex);
    b_len    = 1;
    b_vals[1] = (seq_t)mex;          /* 5 */
    mark_used((idx_t)mex);
    b_len    = 2;
    b_vals[2] = (seq_t)mex;          /* 6 */
    mark_used((idx_t)mex);
    b_len    = 3;
}

/* --- ensure the sequences are computed up to n ------------------------ */
static void ensure_a(idx_t n)
{
    if (a_len > n) return;          /* already available */

    if (a_len == 0) init_cache(n);

    for (idx_t k = a_len; k <= n; ++k) {
        seq_t ak;
        if (k < 3) {                /* initial values already stored */
            ak = a_vals[k];
        } else {
            ak = (seq_t)( (seq_t)2 * b_vals[k-1] +
                          b_vals[k-2] -
                          b_vals[k-3] );
        }

        /* store a(k) */
        if (a_len >= a_cap) {
            idx_t new_cap = a_cap << 1;
            seq_t *tmp;
#ifdef __cplusplus
            tmp = new seq_t[new_cap];
            for (idx_t i = 0; i < a_cap; ++i) tmp[i] = a_vals[i];
            delete[] a_vals;
#else
            tmp = (seq_t *)realloc(a_vals, (size_t)new_cap * sizeof(seq_t));
            if (!tmp) abort();
#endif
            a_vals = tmp;
            a_cap  = new_cap;
        }
        a_vals[a_len++] = ak;
        mark_used((idx_t)ak);

        /* compute b(k) for k ≥ 3 */
        if (k >= 3) {
            if (b_len >= b_cap) {
                idx_t new_cap = b_cap << 1;
                seq_t *tmp;
#ifdef __cplusplus
                tmp = new seq_t[new_cap];
                for (idx_t i = 0; i < b_cap; ++i) tmp[i] = b_vals[i];
                delete[] b_vals;
#else
                tmp = (seq_t *)realloc(b_vals, (size_t)new_cap * sizeof(seq_t));
                if (!tmp) abort();
#endif
                b_vals = tmp;
                b_cap  = new_cap;
            }
            seq_t bk = (seq_t)mex;
            b_vals[b_len++] = bk;
            mark_used((idx_t)bk);
        }
    }
}

/* --- public entry point ------------------------------------------------ */
seq_t a(idx_t n)
{
    ensure_a(n);
    return a_vals[n];
}
