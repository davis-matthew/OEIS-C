/*
 * A298449  Number of nX3 0..1 arrays with every element equal to 1, 3, 4, 5
 *          or 6 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined for n ≥ 1 by the 17‑term recurrence a(n) =
 *            a(n‑1) + 4·a(n‑2) + 21·a(n‑3) – 16·a(n‑4) – 60·a(n‑5) –
 *            83·a(n‑6) + 55·a(n‑7) + 144·a(n‑8) + 125·a(n‑9) + 67·a(n‑10) –
 *            98·a(n‑11) – 105·a(n‑12) – 92·a(n‑13) + 37·a(n‑14) + 11·a(n‑15)
 *            – 7·a(n‑16) + 3·a(n‑17) for n > 18 with the first 18 te...
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 21, u64 n <= 41, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

static const seq_t init_vals[18] = {
    (seq_t)0,    // a(1)
    (seq_t)4,    // a(2)
    (seq_t)0,    // a(3)
    (seq_t)11,   // a(4)
    (seq_t)26,   // a(5)
    (seq_t)46,   // a(6)
    (seq_t)204,  // a(7)
    (seq_t)696,  // a(8)
    (seq_t)1493, // a(9)
    (seq_t)5880, // a(10)
    (seq_t)18994,// a(11)
    (seq_t)49941,// a(12)
    (seq_t)174020,// a(13)
    (seq_t)552946,// a(14)
    (seq_t)1590917,// a(15)
    (seq_t)5237693,// a(16)
    (seq_t)16486804,// a(17)
    (seq_t)49511202 // a(18)
};

static void ensure_cache(idx_t n)
{
    if (n <= cache_size) return;

    idx_t new_size = n + 1;                 /* allocate indices 0..n */
    seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_cache) abort();

    /* copy previously computed values */
    for (idx_t i = 1; i <= cache_size; ++i) new_cache[i] = cache[i];
    if (cache) free(cache);

    /* fill missing entries up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i <= 18) {
            new_cache[i] = init_vals[i - 1];
        } else {
            seq_t s_pos = new_cache[i - 1];
            s_pos += (seq_t)4   * new_cache[i - 2];
            s_pos += (seq_t)21  * new_cache[i - 3];
            s_pos += (seq_t)55  * new_cache[i - 7];
            s_pos += (seq_t)144 * new_cache[i - 8];
            s_pos += (seq_t)125 * new_cache[i - 9];
            s_pos += (seq_t)67  * new_cache[i - 10];
            s_pos += (seq_t)37  * new_cache[i - 14];
            s_pos += (seq_t)11  * new_cache[i - 15];
            s_pos += (seq_t)3   * new_cache[i - 17];

            seq_t s_neg = (seq_t)16  * new_cache[i - 4];
            s_neg += (seq_t)60  * new_cache[i - 5];
            s_neg += (seq_t)83  * new_cache[i - 6];
            s_neg += (seq_t)98  * new_cache[i - 11];
            s_neg += (seq_t)105 * new_cache[i - 12];
            s_neg += (seq_t)92  * new_cache[i - 13];
            s_neg += (seq_t)7   * new_cache[i - 16];

            new_cache[i] = s_pos - s_neg;
        }
    }

    cache = new_cache;
    cache_size = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    ensure_cache(n);
    return cache[n];
}
