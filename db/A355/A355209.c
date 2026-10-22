/*
 * A355209  E.g.f. A(x) satisfies A'(x) = 1 + A(-2 * log(1-x)).
 * https://oeis.org/A355209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(1) = 1;
 *          a(n+1) = Σ_{k=1..n} 2^k * |Stirling1(n,k)| * a(k)
 *  (where Stirling1 are unsigned Stirling numbers of the first kind)
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
/* seq_t and idx_t are supplied by the build. */

static seq_t *a_cache = NULL;      /* cache for a(n) */
static size_t   a_cap   = 0;
static idx_t    a_max   = 0;        /* largest n computed (>=1) */

static seq_t *s_row = NULL;        /* current Stirling row S(n,k) */
static size_t   s_cap  = 0;
static idx_t    s_n    = 0;         /* current n of s_row */

static seq_t *pow2_cache = NULL;   /* 2^k values */
static size_t   pow2_cap   = 0;
static idx_t    pow2_max   = 0;

/* Resize an array of any element type so that it can hold at least `needed` elements. */
static void grow_array(void **p, size_t *cap, size_t elem_size, size_t needed)
{
    if (needed > *cap) {
        size_t new_cap = (*cap ? (*cap << 1) : 8);
        while (new_cap < needed) new_cap <<= 1;
        *p = realloc(*p, new_cap * elem_size);
        *cap = new_cap;
    }
}

/* Ensure that 2^k is known for 0 ≤ k ≤ n. */
static void ensure_pow2(idx_t n)
{
    if (n > pow2_max) {
        size_t needed = n + 1;
        grow_array((void **)&pow2_cache, &pow2_cap, sizeof(seq_t), needed);
        for (idx_t i = pow2_max + 1; i <= n; ++i)
            pow2_cache[i] = pow2_cache[i - 1] * (seq_t)2;
        pow2_max = n;
    }
}

/* Initialise caches on first use. */
static void init(void)
{
    if (!a_cache) {
        grow_array((void **)&a_cache, &a_cap, sizeof(seq_t), 2);
        a_cache[1] = (seq_t)1;      /* a(1) = 1 */
        a_max = 1;
    }
    if (!s_row) {
        grow_array((void **)&s_row, &s_cap, sizeof(seq_t), 2);
        s_row[0] = (seq_t)1;         /* S(0,0) = 1 */
        s_n = 0;
    }
    if (!pow2_cache) {
        grow_array((void **)&pow2_cache, &pow2_cap, sizeof(seq_t), 2);
        pow2_cache[0] = (seq_t)1;    /* 2^0 = 1 */
        pow2_max = 0;
    }
}

/* Extend the sequence up to at least n. */
static void extend_to(idx_t n)
{
    init();
    if (n <= a_max) return;          /* already computed */

    /* Make sure we have powers of two up to n. */
    ensure_pow2(n);

    /* Compute successive a values. */
    for (idx_t cur = a_max; cur < n; ++cur) {
        /* Update Stirling row to n = cur. */
        for (idx_t v = s_n + 1; v <= cur; ++v) {
            if ((idx_t)(s_cap) <= v) grow_array((void **)&s_row, &s_cap,
                                                sizeof(seq_t), v + 1);
            /* compute new row in-place, going from high k to low k */
            for (idx_t k = v; k >= 1; --k) {
                seq_t term_left  = (k >= 1) ? s_row[k - 1] : 0;
                seq_t term_right = (k <= v - 1) ? s_row[k] : 0;
                s_row[k] = term_left + (seq_t)(v - 1) * term_right;
            }
            s_row[0] = 0;                /* S(v,0) = 0 for v>0 */
            s_n = v;
        }

        /* Compute a(cur+1) = Σ_{k=1..cur} 2^k * S(cur, k) * a(k) */
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= cur; ++k) {
            seq_t term = s_row[k];
            term *= pow2_cache[k];
            term *= a_cache[k];
            sum += term;
        }

        grow_array((void **)&a_cache, &a_cap, sizeof(seq_t), cur + 2);
        a_cache[cur + 1] = sum;
    }
    a_max = n;
}

/* Public entry point. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* safe placeholder for offset 1 */
    extend_to(n);
    return a_cache[n];
}
