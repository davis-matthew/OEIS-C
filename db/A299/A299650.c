/*
 * A299650  Number of nX4 0..1 arrays with every element equal to 0, 1, 2, 3,
 *          4, 5 or 6 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A299650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A299650: number of n×4 0..1 arrays with the upper left
 * element zero, where every element equals the number of its king‑move
 * adjacent elements that are equal to it.  The sequence satisfies the
 * 14‑term linear recurrence
 *
 *   a(n) = 13*a(n-1) + 31*a(n-2) + 61*a(n-3) - 43*a(n-4) - 428*a(n-5)
 *          - 273*a(n-6) + 69*a(n-7) + 545*a(n-8) + 212*a(n-9)
 *          - 84*a(n-10) - 204*a(n-11) - 32*a(n-12) + 16*a(n-13)
 *          + 24*a(n-14)
 *
 * with initial values
 *   a(1)=8, a(2)=128, a(3)=1924, a(4)=29408, a(5)=448993,
 *   a(6)=6856789, a(7)=104711327, a(8)=1599074414,
 *   a(9)=24419877459, a(10)=372922269561, a(11)=5694992480926,
 *   a(12)=86969703985622, a(13)=1328136856258967,
 *   a(14)=20282321638150114.
 *
 * The function implements this recurrence lazily and memoises the
 * computed values to allow any order of calls.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;  /* 1‑based cache: seq_cache[n] == a(n) */
static idx_t  seq_max   = 0;     /* largest n for which a(n) is cached */

/* Initial 14 values; seq_cache[0] is unused */
static const seq_t seq_init[15] = {
    0, /* unused, keeps 1‑based indexing */
    (seq_t)8,
    (seq_t)128,
    (seq_t)1924,
    (seq_t)29408,
    (seq_t)448993,
    (seq_t)6856789,
    (seq_t)104711327,
    (seq_t)1599074414,
    (seq_t)24419877459,
    (seq_t)372922269561,
    (seq_t)5694992480926,
    (seq_t)86969703985622,
    (seq_t)1328136856258967,
    (seq_t)20282321638150114
};

seq_t a(idx_t n)
{
    /* n must be >= 1 by specification; return 0 for an illegal call */
    if (n == 0) return (seq_t)0;

    /* Grow cache if necessary */
    if (seq_max < n) {
        /* Ensure enough space (use double growth for efficiency) */
        idx_t new_len = (seq_max + 1) ? seq_max + 1 : 1;
        while (new_len <= n) new_len <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure – unlikely in tests */
        seq_cache = tmp;

        /* If cache was just created, initialise all entries up to n with
         * zeros (seq_cache[0] stays unused). Existing entries > seq_max
         * are already zero due to realloc or calloc semantics. */
    }

    /* Compute missing terms up to n */
    for (idx_t k = seq_max + 1; k <= n; ++k) {
        seq_t value;
        if (k <= 14) {
            value = seq_init[k];
        } else {
            /* Recurrence: a(n) = sum of terms with positive coefficients
             * minus sum of terms with negative coefficients.  The result
             * is guaranteed to be non‑negative. */
            seq_t sum = 0;

            /* Positive coefficient terms */
            sum += (seq_t)13  * seq_cache[k - 1];
            sum += (seq_t)31  * seq_cache[k - 2];
            sum += (seq_t)61  * seq_cache[k - 3];
            sum += (seq_t)69  * seq_cache[k - 7];
            sum += (seq_t)545 * seq_cache[k - 8];
            sum += (seq_t)212 * seq_cache[k - 9];
            sum += (seq_t)16  * seq_cache[k - 13];
            sum += (seq_t)24  * seq_cache[k - 14];

            /* Negative coefficient terms */
            sum -= (seq_t)43  * seq_cache[k - 4];
            sum -= (seq_t)428 * seq_cache[k - 5];
            sum -= (seq_t)273 * seq_cache[k - 6];
            sum -= (seq_t)84  * seq_cache[k - 10];
            sum -= (seq_t)204 * seq_cache[k - 11];
            sum -= (seq_t)32  * seq_cache[k - 12];

            value = sum;
        }

        seq_cache[k] = value;
    }

    seq_max = n;
    return seq_cache[n];
}
