/*
 * A305017  Number of nX3 0..1 arrays with every element unequal to 2, 3, 4, 5
 *          or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A305017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A305017: Number of n×3 0..1 arrays with no equal element in a king‑move
   neighborhood, upper‑left element zero.  This implementation uses the
   16‑term linear recurrence:

   a(n) = 3 a(n‑1) + 14 a(n‑2) + 6 a(n‑3) – 79 a(n‑4) – 104 a(n‑5)
          + 77 a(n‑6) + 181 a(n‑7) + 65 a(n‑8) – 51 a(n‑9) – 5 a(n‑10)
          + 49 a(n‑11) + 5 a(n‑12) – 18 a(n‑13) – 2 a(n‑14) + 4 a(n‑15)
          for n > 16

   Initial values a(1) … a(16) are provided explicitly.
*/
#include <stdlib.h>

static const seq_t init[17] = {
    0ULL,            /* 0 – unused */
    0ULL,            /* a(1)  */
    5ULL,            /* a(2)  */
    14ULL,           /* a(3)  */
    73ULL,           /* a(4)  */
    387ULL,          /* a(5)  */
    2000ULL,         /* a(6)  */
    10487ULL,        /* a(7)  */
    54957ULL,        /* a(8)  */
    287218ULL,       /* a(9)  */
    1503959ULL,      /* a(10) */
    7869877ULL,      /* a(11) */
    41184270ULL,     /* a(12) */
    215540219ULL,    /* a(13) */
    1127976673ULL,   /* a(14) */
    5903110936ULL,   /* a(15) */
    30893060733ULL   /* a(16) */
};

/**
 * Compute a(n) for the OEIS A305017 sequence.
 *
 * @param n index (1‑based).  Returns 0 for n <= 0.
 * @return the value a(n), stored in type seq_t.
 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out‑of‑range safety */

    /* Return immediate values for the first 16 terms. */
    if (n <= 16) return init[n];

    /* Keeps the computed terms from the first call onward. */
    static seq_t *cache = NULL;
    static idx_t cached_len = 0;   /* highest index stored (0 means none) */

    /* Initialise the cache once with the first 16 terms. */
    if (cached_len == 0) {
        cache = (seq_t *)malloc((16 + 1) * sizeof(seq_t));
        for (idx_t i = 1; i <= 16; ++i) cache[i] = init[i];
        cached_len = 16;
    }

    /* Grow the cache if the requested n is beyond the current maximum. */
    if (n > cached_len) {
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) return 0;   /* allocation failure – unlikely in tests */
        cache = new_cache;

        /* Compute missing terms using the recurrence. */
        for (idx_t i = cached_len + 1; i <= n; ++i) {
            seq_t pos = 0;
            pos += (seq_t)3   * cache[i-1];
            pos += (seq_t)14  * cache[i-2];
            pos += (seq_t)6   * cache[i-3];
            pos += (seq_t)77  * cache[i-6];
            pos += (seq_t)181 * cache[i-7];
            pos += (seq_t)65  * cache[i-8];
            pos += (seq_t)49  * cache[i-11];
            pos += (seq_t)5   * cache[i-12];
            pos += (seq_t)4   * cache[i-15];

            seq_t neg = 0;
            neg += (seq_t)79  * cache[i-4];
            neg += (seq_t)104 * cache[i-5];
            neg += (seq_t)51  * cache[i-9];
            neg += (seq_t)5   * cache[i-10];
            neg += (seq_t)18  * cache[i-13];
            neg += (seq_t)2   * cache[i-14];

            cache[i] = pos - neg;   /* final result is positive */
        }
        cached_len = n;
    }

    return cache[n];
}
