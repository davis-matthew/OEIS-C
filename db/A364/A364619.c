/*
 * A364619  Number of 4-cycles in the n-Pell graph.
 * https://oeis.org/A364619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 6*a(n-1)-9*a(n-2)-4*a(n-3)+9*a(n-4)+6*a(n-5)+a(n-6)
 * Initial values: a(0)=0, a(1)=0, a(2)=1, a(3)=8, a(4)=40, a(5)=164
 * The sequence satisfies this linear recurrence for all n ≥ 6.
 * The implementation memoises results up to the largest n requested,
 * making each call O(n) in total and O(1) for repeated or random order
 * queries.
 */

#include <stdlib.h>   /* malloc, realloc */

static seq_t *cache  = NULL;    /* memoisation array */
static size_t cap     = 0;      /* allocated capacity */
static size_t computed = 0;     /* number of entries already computed (>=6) */

/* Initialise cache with first six terms */
static void init_cache(void)
{
    if (cap != 0) return;          /* already initialised */
    const size_t init_cap = 16;
    cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
    cap = init_cap;

    cache[0] = 0;
    cache[1] = 0;
    cache[2] = 1;
    cache[3] = 8;
    cache[4] = 40;
    cache[5] = 164;
    computed = 6;                   /* a(0)..a(5) are ready */
}

/* Grow cache if necessary and fill missing entries using the recurrence */
static void ensure_up_to(idx_t n)
{
    size_t target = (size_t)n;
    init_cache();

    if (target + 1 <= computed)
        return;                    /* already have a(n) */

    /* Grow capacity to at least target+1 */
    if (target + 1 > cap) {
        size_t new_cap = cap;
        while (new_cap < target + 1)
            new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing values */
    for (size_t i = computed; i <= target; ++i) {
        seq_t v = 0;
        v += 6 * cache[i-1];
        v -= 9 * cache[i-2];
        v -= 4 * cache[i-3];
        v += 9 * cache[i-4];
        v += 6 * cache[i-5];
        v += cache[i-6];
        cache[i] = v;
    }
    computed = target + 1;
}

/* Public entry point */
seq_t a(idx_t n)
{
    ensure_up_to(n);
    return cache[(size_t)n];
}
