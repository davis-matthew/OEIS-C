/*
 * A247227  Numbers that divide the sum of the factorials of their digits in
 *          base 10.
 * https://oeis.org/A247227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A247227.
   Definition: a(n) is the n-th natural number that divides the sum of
   the factorials of its decimal digits.  The function searches
   sequentially from 1 upward, caches the found terms and returns
   the n‑th one.  The cache size is fixed at 20000, which is ample for
   n up to about 10000. */
#include <stdlib.h>
#include <stdint.h>

static seq_t        cache[20000];   /* cached sequence values   */
static size_t       cached   = 0;    /* number of cached items   */
static idx_t        next_candidate = 1; /* next integer to test   */
static seq_t        fact[10];       /* factorials 0..9 */

/* Initialise the factorial table (one‑time). */
static void init_factorials(void) {
    if (fact[0] != 0) return;        /* already done */
    fact[0] = 1;
    for (idx_t i = 1; i <= 9; ++i)
        fact[i] = fact[i-1] * (seq_t)i;
}

/* Append a new term into the cache.  No bounds check is performed
   because the cache size is large enough for the required n. */
static void append_term(seq_t val) {
    cache[cached++] = val;
}

/* Return the n-th term of the sequence (1‑based indexing).  */
seq_t a(idx_t n) {
    if (n == 0) return 0;           /* undefined for index 0      */
    init_factorials();              /* ensure factorials ready    */

    /* Generate terms until we have at least n cached. */
    while (cached < n) {
        seq_t sum = 0;
        idx_t t   = next_candidate;

        /* Sum of factorials of decimal digits of next_candidate. */
        if (t == 0) {
            sum = fact[0];
        } else {
            while (t > 0) {
                uint8_t d = (uint8_t)(t % 10);
                sum += fact[d];
                t   /= 10;
            }
        }

        /* Check divisibility. */
        if (sum % (seq_t)next_candidate == 0)
            append_term((seq_t)next_candidate);

        ++next_candidate;
    }
    return cache[n-1];
}
