/*
 * A252250  Number of (n+2) X (4+2) 0..3 arrays with every 3 X 3 subblock row
 *          and column sum not equal to 0 3 5 6 or 7 and every 3 X 3 diagonal
 *          and antidiagonal sum equal to 0 3 5 6 or 7.
 * https://oeis.org/A252250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 25, u32 n <= 59, u64 n <= 128, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-1) + 3*a(n-3) - 3*a(n-4) - a(n-6) + a(n-7)  (n > 9)
 * Initial values:
 *   a(1)=352, a(2)=268, a(3)=249, a(4)=267, a(5)=297,
 *   a(6)=336, a(7)=384, a(8)=462, a(9)=564
 *
 * Implementation uses memoised linear recurrence.  The cache is 1‑based
 * and grows as needed.  The smallest cache size is 10 to store the
 * initial 9 terms.
 */
#include <stdlib.h>
#include <stddef.h>

static seq_t *cache   = NULL;   /* 1‑based cache: cache[1] .. cache[n] */
static size_t cache_size = 0;   /* number of computed entries (>=9 after init) */
static size_t cache_cap  = 0;   /* current allocated capacity (>= cache_size+1) */

/* Ensure cache can hold values up to at least n (1‑based) */
static void ensure_cache(idx_t n)
{
    /* Need room for indices 0..n; plus at least 10 slots for initial terms */
    size_t needed_cap = (size_t)(n + 1);
    if (needed_cap < 10) needed_cap = 10;

    if (cache_cap < needed_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, needed_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);  /* abort on allocation failure */
        cache = tmp;
        cache_cap = needed_cap;
    }

    /* Initialise first nine terms once, if not already done */
    if (cache_size < 9) {
        cache[1] = (seq_t)352;
        cache[2] = (seq_t)268;
        cache[3] = (seq_t)249;
        cache[4] = (seq_t)267;
        cache[5] = (seq_t)297;
        cache[6] = (seq_t)336;
        cache[7] = (seq_t)384;
        cache[8] = (seq_t)462;
        cache[9] = (seq_t)564;
        cache_size = 9;
    }

    /* Compute missing values up to n using the recurrence */
    if (n > cache_size) {
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            seq_t val = cache[(size_t)i - 1];
            val += 3 * cache[(size_t)i - 3];
            val -= 3 * cache[(size_t)i - 4];
            val -= cache[(size_t)i - 6];
            val += cache[(size_t)i - 7];
            cache[(size_t)i] = val;
        }
        cache_size = (size_t)n;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for n < 1 */
    ensure_cache(n);
    return cache[(size_t)n];
}
