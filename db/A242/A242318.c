/*
 * A242318  Number of length n+3+2 0..3 arrays with every value 0..3 appearing
 *          at least once in every consecutive 3+3 elements, and new values
 *          0..3 introduced in order.
 * https://oeis.org/A242318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + 3*a(n-3) + 5*a(n-4) + 6*a(n-5) * -
 *            a(n-6) - a(n-7) - a(n-9) - a(n-10) for n > 10 * Initial values:
 *            * a(1) = 65, a(2) = 185, a(3) = 503, a(4) = 1316, * a(5) = 3398,
 *            a(6) = 8801, a(7) = 23069, a(8) = 60197, * a(9) = 156887, a(10)
 *            = 408962
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 19, u64 n <= 42, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t base_vals[11] = {
    0ULL,          /* index 0 unused */
    65ULL,         /* a(1) */
    185ULL,        /* a(2) */
    503ULL,        /* a(3) */
    1316ULL,       /* a(4) */
    3398ULL,       /* a(5) */
    8801ULL,       /* a(6) */
    23069ULL,      /* a(7) */
    60197ULL,      /* a(8) */
    156887ULL,     /* a(9) */
    408962ULL      /* a(10) */
};

seq_t a(idx_t n)
{
    /* n is unsigned; behaviour for n < 1 is undefined but not expected */
    if (n == 0) return 0;

    /* Return a pre‑computed initial value if we are within the base range. */
    if (n <= 10) return base_vals[n];

    /* Static state for memoisation. */
    static seq_t *memo  = NULL;   /* pointer to cached values */
    static idx_t  cap   = 0;      /* current allocation size (>= memo_max+1) */
    static idx_t  memo_max = 0;   /* largest index for which memo[i] is defined */

    /* Allocate / resize the cache if necessary. */
    if (memo == NULL) {
        /* First time: allocate up to n (or at least 10). */
        cap = (n > 10 ? n : 10);
        memo = (seq_t *)malloc(sizeof(seq_t) * (cap + 1));
        if (memo == NULL) return 0; /* allocation failure – unlikely */
        /* Initialise the base values. */
        memo[0] = 0;
        for (idx_t i = 1; i <= 10; ++i) memo[i] = base_vals[i];
        memo_max = 10;
    } else if (n > cap) {
        /* Grow the cache to accommodate the new maximum index. */
        idx_t new_cap = n;
        seq_t *new_mem = (seq_t *)realloc(memo, sizeof(seq_t) * (new_cap + 1));
        if (new_mem == NULL) return 0; /* allocation failure – unlikely */
        memo = new_mem;
        cap = new_cap;
        /* No new values are known yet; memo_max remains unchanged. */
    }

    /* Compute all missing terms up to n. */
    if (n > memo_max) {
        for (idx_t i = memo_max + 1; i <= n; ++i) {
            seq_t val = memo[i-1];
            val += memo[i-2] * 2;
            val += memo[i-3] * 3;
            val += memo[i-4] * 5;
            val += memo[i-5] * 6;
            val -= memo[i-6];
            val -= memo[i-7];
            val -= memo[i-9];
            val -= memo[i-10];
            memo[i] = val;
        }
        memo_max = n;
    }

    return memo[n];
}
