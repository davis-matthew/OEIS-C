/*
 * A298006  Solution b( ) of the complementary equation a(n) = a(1)*b(n-1) -
 *          a(0)*b(n-2) + 2*n - 1, where a(0) = 1, a(1) = 2, b(0) = 3, b(1) =
 *          4, and (b(n)) is the increasing sequence of positive integers not
 *          in (a(n)). See Comments.
 * https://oeis.org/A298006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(1)*b(n‑1) – a(0)*b(n‑2) + 2*n – 1 * with a(0)=1,
 *            a(1)=2, b(0)=3, b(1)=4, and (b(n)) the increasing sequence * of
 *            positive integers not in (a(n)). This function returns the nth
 *            term * of that b sequence
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static seq_t *a_cache = NULL;          /* cache of a(i) values */
static seq_t *b_cache = NULL;          /* cache of b(i) values */
static idx_t cache_cap = 0;            /* allocated size of caches */
static idx_t cache_len = 0;            /* highest index stored + 1 */

static unsigned char *used = NULL;     /* bitset: 1 if value seen in a or b */
static idx_t used_cap = 0;             /* allocated size of used array */

static void ensure_cache(idx_t up_to)
{
    if (up_to + 1 <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 4;
    if (new_cap < up_to + 1) new_cap = up_to + 1;
    a_cache = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
    b_cache = (seq_t*)realloc(b_cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

static void ensure_used(idx_t val)
{
    if (val < used_cap) return;
    idx_t new_cap = used_cap ? used_cap * 2 : 16;
    while (new_cap <= val) new_cap <<= 1;
    unsigned char *new_used = (unsigned char*)calloc(new_cap, 1);
    if (used) {
        memcpy(new_used, used, used_cap);
        free(used);
    }
    used = new_used;
    used_cap = new_cap;
}

static inline bool is_used(idx_t v)
{
    return v < used_cap && used[v];
}

static inline void mark_used(idx_t v)
{
    if (v >= used_cap) ensure_used(v);
    used[v] = 1;
}

/* Computes the nth term of the sequence b (OEIS A298006) */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        /* initial capacity for caches */
        cache_cap = 4;
        a_cache = (seq_t*)calloc(cache_cap, sizeof(seq_t));
        b_cache = (seq_t*)calloc(cache_cap, sizeof(seq_t));

        /* initial terms */
        a_cache[0] = (seq_t)1;
        a_cache[1] = (seq_t)2;
        b_cache[0] = (seq_t)3;
        b_cache[1] = (seq_t)4;

        /* used array for membership tests */
        used_cap = 16;
        used = (unsigned char*)calloc(used_cap, 1);
        mark_used((idx_t)1);
        mark_used((idx_t)2);
        mark_used((idx_t)3);
        mark_used((idx_t)4);

        cache_len = 2;          /* indices 0 and 1 are set */
        initialized = true;
    }

    if (n >= cache_len) {
        ensure_cache(n);
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i < 2) continue;   /* already initialized */

            /* compute a[i] from the recurrence */
            seq_t an = (seq_t)2 * b_cache[i-1] - b_cache[i-2]
                         + (seq_t)2 * (seq_t)i - (seq_t)1;
            a_cache[i] = an;
            /* mark a[i] as used if it fits into the used array range */
            if ((idx_t)an < 1ULL << 63)   /* safe cast for typical builds */
                mark_used((idx_t)an);

            /* find next b[i] as the smallest unused integer > b[i-1] */
            idx_t cand = (idx_t)b_cache[i-1] + 1;
            while (is_used(cand)) ++cand;
            b_cache[i] = (seq_t)cand;
            mark_used(cand);
        }
        cache_len = n + 1;
    }

    return b_cache[n];
}
