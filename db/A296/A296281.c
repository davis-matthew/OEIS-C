/*
 * A296281  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-2)*b(n-1)*b(n), where a(0) = 2, a(1) = 3, b(0) = 1, b(1) = 4,
 *          b(2) = 5, and (a(n)) and (b(n)) are increasing complementary
 *          sequences.
 * https://oeis.org/A296281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 35, u64 n <= 81, u128 n <= 173,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) for A296281:  
   a(0) = 2, a(1) = 3,  
   a(n) = a(n-1) + a(n-2) + b(n-2)*b(n-1)*b(n)  for n ≥ 2,  
   where b() is the complementary increasing sequence (1,4,5,6,7,8,…).  
   The sequence values grow rapidly; this implementation builds a(n) and
   its complement b(n) on demand and caches the results. */

#include <stdlib.h>

/* ----- static storage for the computed terms ----- */
static seq_t *a_vals = NULL;       /* a[0] … a[a_len-1] */
static seq_t *b_vals = NULL;       /* b[0] … b[b_len-1] */
static size_t   a_cap  = 0;        /* capacity of a_vals */
static size_t   b_cap  = 0;        /* capacity of b_vals */
static idx_t    a_len  = 0;        /* number of a values stored */
static idx_t    b_len  = 0;        /* number of b values stored */
static seq_t   last_b_candidate = 0;  /* last integer considered for b */
static idx_t   b_check_pos = 0;       /* position in a_vals for b search */

/* ----- helper: obtain the next unused positive integer, skipping all
          a-values already known. The result is appended to b_vals. ----- */
static seq_t get_next_b(void)
{
    seq_t cand;
    while (1) {
        cand = ++last_b_candidate;            /* next integer to test    */
        /* advance b_check_pos while a_vals[b_check_pos] < cand   */
        while (b_check_pos < a_len && a_vals[b_check_pos] < cand)
            ++b_check_pos;
        /* if cand equals an existing a-value, skip it */
        if (b_check_pos < a_len && a_vals[b_check_pos] == cand) {
            ++b_check_pos;      /* move past the matched a-value     */
            continue;           /* try the next candidate            */
        }
        return cand;           /* cand is the next b-value           */
    }
}

/* ----- ensure that a_vals[0..n] and b_vals[0..n] are computed ----- */
static void ensure_cache(idx_t n)
{
    /* ---- allocate/expand storage as needed ---- */
    if (!a_vals) {
        a_cap = 4;
        a_vals = (seq_t *)malloc(a_cap * sizeof(seq_t));
    }
    if (!b_vals) {
        b_cap = 4;
        b_vals = (seq_t *)malloc(b_cap * sizeof(seq_t));
    }

    /* ---- store the initial a-values if missing ---- */
    if (a_len == 0) {
        a_vals[0] = (seq_t)2;
        ++a_len;
    }
    if (a_len == 1) {
        a_vals[1] = (seq_t)3;
        ++a_len;
    }

    /* ---- generate b-values up to index n ---- */
    while (b_len <= n) {
        seq_t next = get_next_b();
        if (b_len == b_cap)
            b_vals = (seq_t *)realloc(b_vals, (b_cap = b_cap ? b_cap * 2U : 4U) * sizeof(seq_t));
        b_vals[b_len++] = next;
    }

    /* ---- compute a-values up to index n ---- */
    for (idx_t i = a_len; i <= n; ++i) {
        /* indices i-2, i-1, i are guaranteed to exist in b_vals */
        seq_t prod = b_vals[i-2] * b_vals[i-1] * b_vals[i];
        seq_t ai   = a_vals[i-1] + a_vals[i-2] + prod;
        if (a_len == a_cap)
            a_vals = (seq_t *)realloc(a_vals, (a_cap = a_cap ? a_cap * 2U : 4U) * sizeof(seq_t));
        a_vals[a_len++] = ai;
    }
}

/* ----- public entry point ----- */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return a_vals[n];
}
