/*
 * A252807  Number of (n+2) X (4+2) 0..4 arrays with every consecutive three
 *          elements in every row and column not having exactly two distinct
 *          values, and in every diagonal and antidiagonal having exactly two
 *          distinct values, and new values 0 upwards introduced in row major
 *          order.
 * https://oeis.org/A252807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-3) - 4*a(n-6) - 2*a(n-9) + a(n-12) (n > 15) * a(1)
 *            = 752, a(2) = 369, a(3) = 551, a(4) = 742, a(5) = 1625, * a(6) =
 *            2795, a(7) = 3763, a(8) = 8275, a(9) = 14320, * a(10) = 19355,
 *            a(11) = 42636, a(12) = 73806, a(13) = 99746, * a(14) = 219780,
 *            a(15) = 380509. The recurrence is linear w...
 * Range:     u8 none, u16 n <= 11, u32 n <= 32, u64 n <= 73, u128 n <= 154,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const seq_t init[15] = {
    752ULL, 369ULL, 551ULL, 742ULL, 1625ULL,
    2795ULL, 3763ULL, 8275ULL, 14320ULL, 19355ULL,
    42636ULL, 73806ULL, 99746ULL, 219780ULL, 380509ULL
};

static seq_t *cache = NULL;  /* indexed from 1 to current max_n */
static size_t max_n = 0;     /* largest index with a computed value */

/* Return a(n) for n >= 1 */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n < 1 */

    size_t N = (size_t)n;  /* for array indexing */

    /* Grow the cache if necessary */
    if (N > max_n) {
        size_t new_cap = N + 1;          /* index 0 unused */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failure */
        cache = tmp;
        /* initialise newly allocated part */
        for (size_t i = max_n + 1; i <= N; ++i) {
            if (i <= 15)
                cache[i] = init[i-1];
            else
                cache[i] = ( seq_t)(6 * cache[i-3]
                                  - 4 * cache[i-6]
                                  - 2 * cache[i-9]
                                  +   cache[i-12] );
        }
        max_n = N;
    }
    return cache[N];
}
