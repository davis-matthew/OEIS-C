/*
 * A320099  Number of no-leaf subgraphs of the 5 X n grid.
 * https://oeis.org/A320099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A320099: Number of no‑leaf subgraphs of the 5 × n grid
 *
 *  Formula (recurrence, n>10):
 *  a(n) = 103*a(n-1) + 1063*a(n-2) - 1873*a(n-3) - 20274*a(n-4)
 *         + 44071*a(n-5) - 10365*a(n-6) - 20208*a(n-7)
 *         + 5959*a(n-8) + 2300*a(n-9) - 500*a(n-10)
 *  Initial terms:
 *  a(1)=1,  a(2)=50,   a(3)=5193,     a(4)=583199,
 *  a(5)=65485654, a(6)=7354266811, a(7)=825905301851,
 *  a(8)=92751581627976, a(9)=10416273692997679,
 *  a(10)=1169777980482365913
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* undefined for n<1, guard only */

    /* Static cache of already computed terms */
    static seq_t *cache    = NULL;
    static size_t cap      = 0;
    static idx_t max_computed = 0;

    /* Initial 10 terms of the sequence */
    static const seq_t init[10] = {
        1ULL,
        50ULL,
        5193ULL,
        583199ULL,
        65485654ULL,
        7354266811ULL,
        825905301851ULL,
        92751581627976ULL,
        10416273692997679ULL,
        1169777980482365913ULL
    };

    /* lazy initialization of the cache */
    if (cache == NULL) {
        cap = 10;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        for (idx_t i = 0; i < 10; ++i)
            cache[i] = init[i];
        max_computed = 10;
    }

    /* Return if the requested term is already computed */
    if (n <= max_computed)
        return cache[n - 1];

    /* Compute terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* Ensure the cache is large enough */
        if (i > cap) {
            size_t new_cap = cap ? (cap << 1) : 16;
            while (i > new_cap) new_cap <<= 1;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cap = new_cap;
        }

        /* Recurrence (valid for i >= 11) */
        seq_t pos = cache[i - 2] * (seq_t)103;
        pos += cache[i - 3] * (seq_t)1063;
        pos += cache[i - 6] * (seq_t)44071;
        pos += cache[i - 9] * (seq_t)5959;
        pos += cache[i -10] * (seq_t)2300;

        seq_t neg = cache[i - 4] * (seq_t)1873;
        neg += cache[i - 5] * (seq_t)20274;
        neg += cache[i - 7] * (seq_t)10365;
        neg += cache[i - 8] * (seq_t)20208;
        neg += cache[i -11] * (seq_t)500;

        cache[i - 1] = pos - neg;   /* pos > neg for all valid n */
    }

    max_computed = n;
    return cache[n - 1];
}
