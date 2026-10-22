/*
 * A305010  Number of nX3 0..1 arrays with every element unequal to 1, 2, 4, 5
 *          or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A305010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by the recurrence a(n) = a(n-1) + 3*a(n-2) +
 *            13*a(n-3) - 9*a(n-4) - 37*a(n-5) - 59*a(n-6) + 27*a(n-7) +
 *            130*a(n-8) + 92*a(n-9) - 35*a(n-10) - 129*a(n-11) - 8*a(n-12) +
 *            78*a(n-13) + 45*a(n-14) - 53*a(n-15) - 34*a(n-16) - 24*a(n-17)
 *            for n > 18 with initial values a(1)=1, a(2)=11, a(...
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 48, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Cached values */
    static seq_t *cache = NULL;          /* 1‑indexed array of computed terms   */
    static size_t cache_len = 0;         /* number of allocated slots           */
    static idx_t computed = 18;          /* highest index that has been computed */

    /* Initialisation: allocate a small cache and fill the first 18 terms */
    if (cache == NULL) {
        /* Allocate a buffer large enough for at least 64 terms */
        cache_len = 64;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (cache == NULL) return 0; /* allocation failed */

        /* Raw base values as 64‑bit constants */
        static const uint64_t base_raw[] = {
            0ULL, /* unused 0‑th index */
            1ULL, 11ULL, 14ULL, 31ULL, 99ULL, 206ULL, 455ULL,
            1321ULL, 3108ULL, 7353ULL, 19511ULL, 48532ULL,
            118463ULL, 303897ULL, 765930ULL, 1903657ULL,
            4822199ULL, 12178658ULL
        };
        const size_t base_size = sizeof(base_raw) / sizeof(base_raw[0]);

        for (size_t i = 1; i < base_size; ++i) {
            cache[i] = (seq_t)base_raw[i];
        }
    }

    /* Grow cache if necessary */
    if ((size_t)n >= cache_len) {
        size_t new_len = cache_len;
        while (new_len <= (size_t)n) new_len <<= 1;  /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (tmp == NULL) return cache[n];          /* cannot grow, return what we have */
        cache = tmp;
        cache_len = new_len;
    }

    /* If already computed, return immediately */
    if (n <= computed) return cache[n];

    /* Compute from the next index up to n */
    for (idx_t idx = computed + 1; idx <= n; ++idx) {
        /* Positive part of the recurrence */
        seq_t sum_pos = cache[idx - 1];
        sum_pos += cache[idx - 2] * (seq_t)3;
        sum_pos += cache[idx - 3] * (seq_t)13;
        sum_pos += cache[idx - 7] * (seq_t)27;
        sum_pos += cache[idx - 8] * (seq_t)130;
        sum_pos += cache[idx - 9] * (seq_t)92;
        sum_pos += cache[idx -13] * (seq_t)78;
        sum_pos += cache[idx -14] * (seq_t)45;

        /* Negative part of the recurrence */
        seq_t sum_neg = cache[idx - 4] * (seq_t)9;
        sum_neg += cache[idx - 5] * (seq_t)37;
        sum_neg += cache[idx - 6] * (seq_t)59;
        sum_neg += cache[idx -10] * (seq_t)35;
        sum_neg += cache[idx -11] * (seq_t)129;
        sum_neg += cache[idx -12] * (seq_t)8;
        sum_neg += cache[idx -15] * (seq_t)53;
        sum_neg += cache[idx -16] * (seq_t)34;
        sum_neg += cache[idx -17] * (seq_t)24;

        cache[idx] = sum_pos - sum_neg;
    }

    computed = n;
    return cache[n];
}
