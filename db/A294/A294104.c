/*
 * A294104  Sum of the differences of the larger and smaller parts in the
 *          partitions of n into two squarefree parts or two nonsquarefree
 *          parts.
 * https://oeis.org/A294104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = Sum_{i=1..floor(n/2)} (n-2i) *
 *            [squarefree(i)=squarefree(n-i)]
 * Range:     u8 n <= 35, u16 n <= 575, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation recomputes the square‑free indicator array up to the
   requested n on demand.  The array is stored in a static buffer and
   rebuilt whenever a larger value of n is requested.  The sum is
   computed in seq_t, using only integer arithmetic. */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>   /* for sqrt */

static uint8_t *squarefree_cache = NULL;  /* 1 = squarefree, 0 = not */
static idx_t squarefree_size = 0;

/* Rebuild the squarefree indicator array up to the given limit.  The old
   cache is freed and the new array is freshly allocated, which keeps the
   logic simple and guarantees correct values for any n. */
static void ensure_squarefree(idx_t limit)
{
    if (squarefree_size >= limit + 1)
        return;                            /* already large enough */

    if (squarefree_cache)
        free(squarefree_cache);

    squarefree_cache = (uint8_t*)malloc((size_t)(limit + 1));
    if (!squarefree_cache)
        abort();                          /* allocation failure */

    squarefree_size = limit + 1;

    /* Initially assume all positive integers are squarefree. */
    for (idx_t i = 1; i <= limit; ++i)
        squarefree_cache[i] = 1;

    /* Mark numbers divisible by a square as not squarefree. */
    idx_t root = (idx_t)sqrt((long double)limit);
    for (idx_t p = 2; p <= root; ++p) {
        idx_t sq = p * p;
        for (idx_t j = sq; j <= limit; j += sq)
            squarefree_cache[j] = 0;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                /* offset starts at 1 */

    ensure_squarefree(n);

    seq_t sum = 0;
    for (idx_t i = 1; i <= n / 2; ++i) {
        if (squarefree_cache[i] == squarefree_cache[n - i]) {
            seq_t diff = (seq_t)n - (seq_t)(2 * i);
            sum += diff;
        }
    }
    return sum;
}
