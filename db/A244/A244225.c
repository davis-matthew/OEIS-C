/*
 * A244225  a(n) = Number of nonnegative integers 0 <= k <= n, which have an
 *          odd representation in Greedy Catalan Base (A014418).
 * https://oeis.org/A244225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - A244229(n) This implementation counts, for each k ≤
 *            n, whether its greedy Catalan base representation contains the
 *            digit “1”. The values are stored incrementally so that arbitrary
 *            calls to a() in any order are fast
 * Range:     u8 n <= 475, u16 n <= 4862, u32 n <= 4862, u64 n <= 4862,
 *            u128 n <= 4862, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *catalan = NULL;    /* Catalan numbers C1, C2, ... */
static size_t cat_len = 0;       /* number of stored Catalan numbers */

static seq_t *a_cache = NULL;    /* a(0), a(1), … */
static size_t a_cap = 0;         /* capacity of a_cache */
static idx_t  a_len = 0;         /* next free index in a_cache */

/* Ensure that catalan[] contains all Catalan numbers ≤ max_val. */
static void ensure_catalan_up_to(seq_t max_val)
{
    if (cat_len == 0) {
        catalan = (seq_t*)malloc(sizeof(seq_t));
        cat_len  = 1;
        catalan[0] = (seq_t)1;            /* C1 = 1 */
    }
    /* Extend until last stored Catalan number > max_val. */
    while (cat_len > 0 && catalan[cat_len-1] <= max_val) {
        /* Next Catalan number: C_{k+1} = C_k * 2*(2k+1) / (k+2)   */
        idx_t k   = (idx_t)cat_len;               /* current index (C_k)   */
        seq_t prev= catalan[cat_len-1];           /* C_k                  */
        seq_t num = (seq_t)(2 * (2 * k + 1));     /* 2*(2k+1)             */
        seq_t den = (seq_t)(k + 2);               /* k+2                   */
        seq_t next = prev * num / den;            /* C_{k+1}              */

        size_t new_cap = cat_len * 2 + 1;
        seq_t *tmp = (seq_t*)realloc(catalan, new_cap * sizeof(seq_t));
        catalan = tmp;
        catalan[cat_len] = next;
        ++cat_len;
    }
}

/* Return the largest index j such that catalan[j] ≤ x.  The array is
   assumed to contain at least one element (C1 = 1). */
static size_t find_last_leq(seq_t x)
{
    size_t i = cat_len-1;
    while (i > 0 && x < catalan[i]) --i;
    return i;
}

/* Return 1 iff the greedy Catalan base representation of n contains
   the digit “1”. */
static int has_1_rep(seq_t n)
{
    seq_t rem = n;
    int used = 0;
    while (rem > 0) {
        size_t idx = find_last_leq(rem);
        seq_t val  = catalan[idx];
        if (val == (seq_t)1) used = 1;
        rem -= val;
    }
    return used;
}

/* Compute a(0)…a(n) and store them in a_cache. */
static void ensure_a_cache_upto(idx_t n)
{
    if (n < a_len) return;                 /* already computed */

    /* Make enough capacity. */
    if (a_cap < (size_t)(n+1)) {
        size_t new_cap = a_cap == 0 ? 16 : a_cap;
        while (new_cap <= (size_t)(n+1)) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
        a_cache = tmp;
        a_cap   = new_cap;
    }

    /* Ensure necessary Catalan numbers exist. */
    ensure_catalan_up_to((seq_t)n);

    /* Compute missing a(i) values. */
    for (idx_t i = a_len; i <= n; ++i) {
        int odd = has_1_rep((seq_t)i);
        if (i == 0) {
            a_cache[0] = 0;
        } else {
            a_cache[i] = a_cache[i-1] + odd;
        }
    }
    a_len = n + 1;
}

/* public function as required */
seq_t a(idx_t n)
{
    ensure_a_cache_upto(n);
    return a_cache[n];
}
