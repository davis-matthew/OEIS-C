/*
 * A250145  Number of length 6+2 0..n arrays with the medians of every three
 *          consecutive terms nondecreasing.
 * https://oeis.org/A250145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 126*a(n-4) *
 *            +126*a(n-5) - 84*a(n-6) + 36*a(n-7) - 9*a(n-8) + a(n-9) * (valid
 *            for n > 10) * Initial values (n = 1 … 10): * 111, 2026, 16104,
 *            81664, 311498, 974944, 2637228, * 6376143, 14100493, 28998090
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 20, u64 n <= 162, u128 n <= 162,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[11] = {
    0,          /* unused 0‑th element for 1‑based indexing */
    111ULL,     /* a(1) */
    2026ULL,    /* a(2) */
    16104ULL,   /* a(3) */
    81664ULL,   /* a(4) */
    311498ULL,  /* a(5) */
    974944ULL,  /* a(6) */
    2637228ULL, /* a(7) */
    6376143ULL, /* a(8) */
    14100493ULL,/* a(9) */
    28998090ULL /* a(10) */
};

static seq_t *cache = NULL;        /* sequence storage, 1‑based */
static idx_t  cache_size = 0;      /* highest index cached */

/*  Computes the n‑th term of OEIS A250145.
 *  The function is memoised: earlier results are reused on later calls.
 *
 *  Note: For very large n the sequence grows ~ n^8/12; values will overflow
 *  built‑in integer types once they exceed their range. For builds with
 *  arbitrary‑precision seq_t the values are exact.  If n<1 the function
 *  returns 0, though the sequence is defined only for n≥1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* out of bounds, not part of the sequence */

    /* Expand cache if necessary */
    if (n > cache_size) {
        /* Allocate or grow the cache array to hold indices 1 … n */
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed: return 0 */
        cache          = tmp;
        cache_size     = n;

        /* Compute new entries */
        for (idx_t i = 1; i <= n; ++i) {
            if (i <= 10) {
                cache[i] = init_vals[i];
            } else {
                /* recurrence for i ≥ 11 */
                seq_t val = 9ULL * cache[i-1];
                val -= 36ULL * cache[i-2];
                val += 84ULL * cache[i-3];
                val -= 126ULL * cache[i-4];
                val += 126ULL * cache[i-5];
                val -= 84ULL * cache[i-6];
                val += 36ULL * cache[i-7];
                val -= 9ULL * cache[i-8];
                val += cache[i-9];
                cache[i] = val;
            }
        }
    }

    return cache[n];
}
