/*
 * A296250  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n)^2, where a(0) = 3, a(1) = 4, b(0) = 1, b(1) = 2, b(2) = 5,
 *          and (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A296250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 38, u64 n <= 84, u128 n <= 176,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A296250: a(n) = a(n‑1) + a(n‑2) + b(n)²
   with a(0)=3, a(1)=4 and complementary increasing sequences a() and b().
   The b sequence is obtained as the list of positive integers not appearing
   in a().  The implementation builds both sequences incrementally and
   memoises all computed terms.  It works for any seq_t, idx_t and keeps
   intermediate values as small as possible. */
#include <stddef.h>   /* for size_t */

/* cache for a(n) values */
static seq_t *a_cache      = NULL;
static size_t a_cap        = 0;
static idx_t  a_cached_up  = 1;         /* highest a indexed cached */

/* cache for b(n) values (b(0)=1, b(1)=2) */
static seq_t *b_cache      = NULL;
static size_t b_cap        = 0;
static idx_t  b_cached_up  = 1;         /* highest b indexed cached */

/* next candidate integer to be tested as a member of b */
static idx_t next_candidate = 3;        /* begins after 1 and 2 */

/* helper: ensure cache for a[] has space for indices up to idx */
static void ensure_a_cap(idx_t idx)
{
    if ((size_t)idx + 1 <= a_cap) return;
    size_t new_cap = a_cap ? a_cap * 2 : 2;
    while (new_cap <= (size_t)idx + 1) new_cap *= 2;
    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    a_cap = new_cap;
}

/* helper: ensure cache for b[] has space for indices up to idx */
static void ensure_b_cap(idx_t idx)
{
    if ((size_t)idx + 1 <= b_cap) return;
    size_t new_cap = b_cap ? b_cap * 2 : 2;
    while (new_cap <= (size_t)idx + 1) new_cap *= 2;
    b_cache = (seq_t *)realloc(b_cache, new_cap * sizeof(seq_t));
    b_cap = new_cap;
}

/* initialise caches on first call */
static void init_caches(void)
{
    static int init_done = 0;
    if (init_done) return;
    init_done = 1;

    /* a(0)=3, a(1)=4 */
    a_cache = (seq_t *)malloc(2 * sizeof(seq_t));
    a_cache[0] = (seq_t)3;
    a_cache[1] = (seq_t)4;
    a_cap = 2;
    a_cached_up = 1;

    /* b(0)=1, b(1)=2 */
    b_cache = (seq_t *)malloc(2 * sizeof(seq_t));
    b_cache[0] = (seq_t)1;
    b_cache[1] = (seq_t)2;
    b_cap = 2;
    b_cached_up = 1;

    next_candidate = 3;
}

/* test whether integer x is among the already computed a[0..last] values */
static int a_contains(idx_t last, seq_t x)
{
    for (idx_t i = 0; i <= last; ++i) {
        if (a_cache[i] == x) return 1;
        /* a_cache is increasing; stop early if passed */
        if (a_cache[i] > x) break;
    }
    return 0;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    init_caches();

    /* if already computed, just return */
    if (n <= a_cached_up) return a_cache[n];

    /* compute up to n */
    for (idx_t i = a_cached_up + 1; i <= n; ++i) {
        /* find b(i): the i‑th integer not in a[0..i‑1] */
        seq_t bi;
        while (1) {
            bi = (seq_t)next_candidate;
            if (!a_contains(i - 1, bi)) break;   /* bi is missing */
            ++next_candidate;
        }
        /* store bi in b_cache */
        ensure_b_cap(i);
        b_cache[i] = bi;
        /* prepare candidate for next search */
        next_candidate = (idx_t)(bi + (seq_t)1);

        /* compute a[i] */
        ensure_a_cap(i);
        a_cache[i] = a_cache[i - 1] + a_cache[i - 2] + bi * bi;
    }

    a_cached_up = n;
    return a_cache[n];
}
