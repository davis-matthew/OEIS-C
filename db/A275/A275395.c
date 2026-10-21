/*
 * A275395  Number of n X 2 0..2 arrays with no element equal to any value at
 *          offset (-2,-1) (-2,0) or (-1,1) and new values introduced in order
 *          0..2.
 * https://oeis.org/A275395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 61, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A275395: Number of n × 2 arrays over {0,1,2} with no entry equal to
 * any value at the offsets (-2,-1), (-2,0) or (-1,1), where new values are introduced
 * in order 0..2.
 *
 * Recurrence (valid for n > 10):
 *   a(n) = 2*a(n-1) + a(n-2) - 3*a(n-3) + a(n-4) + 4*a(n-5)
 *          - 3*a(n-6) + a(n-7) - 2*a(n-8) + a(n-9)
 * Initial values:
 *   a(1) = 2,  a(2) = 9,  a(3) = 16, a(4) = 31,
 *   a(5) = 63, a(6) = 129, a(7) = 260, a(8) = 534,
 *   a(9) = 1083, a(10) = 2210
 *
 * The function memoises computed terms for fast repeated queries.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *g_cache = NULL;        /* array of computed terms */
static size_t g_cap   = 0;           /* allocated capacity          */
static size_t g_len   = 0;           /* number of terms stored      */

static void init_cache(void)
{
    /* Lazily allocate cache and fill the first ten terms. */
    static const seq_t init[10] = {
        (seq_t)2,  (seq_t)9,  (seq_t)16, (seq_t)31,
        (seq_t)63, (seq_t)129,(seq_t)260,(seq_t)534,
        (seq_t)1083,(seq_t)2210
    };

    g_cache = (seq_t *)malloc(10 * sizeof(seq_t));
    if (g_cache == NULL) return;         /* allocation failure */

    for (size_t i = 0; i < 10; ++i)
        g_cache[i] = init[i];

    g_cap = 10;
    g_len = 10;
}

static void ensure_capacity(idx_t n)
{
    if (g_cache == NULL) init_cache();
    if (g_cache == NULL) return;          /* still failed */

    size_t need = (size_t)n;
    if (g_cap >= need) return;            /* already enough */

    size_t new_cap = g_cap ? g_cap : 1;
    while (new_cap < need) new_cap <<= 1; /* grow geometrically */

    seq_t *tmp = (seq_t *)realloc(g_cache, new_cap * sizeof(seq_t));
    if (tmp == NULL) return;               /* reallocation failed */
    g_cache = tmp;
    g_cap   = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                  /* undefined for n < 1 */

    if (g_cache == NULL) init_cache();
    if (g_cache == NULL) return 0;        /* allocation failure */

    /* If the term has already been computed, return it. */
    if ((size_t)n <= g_len) return g_cache[n-1];

    /* Make sure the array is big enough. */
    ensure_capacity(n);
    if (g_cache == NULL) return 0;        /* failure during resize */

    /* Compute terms from g_len+1 up to n. */
    for (idx_t i = (idx_t)(g_len + 1); i <= n; ++i) {
        /* recurrence uses terms a(i-1) .. a(i-9) */
        seq_t v = 0;
        v += (seq_t)2 * g_cache[i-2];   /* 2*a(i-1)  */
        v += g_cache[i-3];              /* +a(i-2)   */
        v -= (seq_t)3 * g_cache[i-4];   /* -3*a(i-3) */
        v += g_cache[i-5];              /* +a(i-4)   */
        v += (seq_t)4 * g_cache[i-6];   /* +4*a(i-5) */
        v -= (seq_t)3 * g_cache[i-7];   /* -3*a(i-6) */
        v += g_cache[i-8];              /* +a(i-7)   */
        v -= (seq_t)2 * g_cache[i-9];   /* -2*a(i-8) */
        v += g_cache[i-10];             /* +a(i-9)   */
        g_cache[i-1] = v;
    }

    g_len = (size_t)n;
    return g_cache[n-1];
}
