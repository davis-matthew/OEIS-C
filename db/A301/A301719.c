/*
 * A301719  Partial sums of A301718.
 * https://oeis.org/A301719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 152, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2*a(n-1) - a(n-2) + a(n-5) - 2*a(n-6) + a(n-7)  for n>6
 * Initial values:
 * a(0)=1, a(1)=6, a(2)=17, a(3)=34, a(4)=57, a(5)=85, a(6)=118, a(7)=157
 */
#include <stddef.h>

#define A301719_MAX_CAP 20000   /* enough to cover the required range */

static seq_t  cache[A301719_MAX_CAP];
static size_t computed = 0;

/* initialise the first eight known terms */
static void initialize_cache(void)
{
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)6;
    cache[2] = (seq_t)17;
    cache[3] = (seq_t)34;
    cache[4] = (seq_t)57;
    cache[5] = (seq_t)85;
    cache[6] = (seq_t)118;
    cache[7] = (seq_t)157;
    computed = 8;
}

/* compute a(n) with memoisation */
seq_t a(idx_t n)
{
    /* Initialise cache on the first call. */
    if (computed == 0)
        initialize_cache();

    /* Return cached value if already computed. */
    if ((size_t)n < computed)
        return cache[(size_t)n];

    /* Compute missing terms up to n. */
    for (size_t i = computed; i <= (size_t)n; ++i) {
        seq_t term = (seq_t)2 * cache[i - 1];   /* 2*a(i-1) */
        term -= cache[i - 2];                   /* -a(i-2) */
        term += cache[i - 5];                   /* +a(i-5) */
        term -= (seq_t)2 * cache[i - 6];        /* -2*a(i-6) */
        term += cache[i - 7];                   /* +a(i-7) */
        cache[i] = term;
    }
    computed = (size_t)n + 1;
    return cache[(size_t)n];
}
