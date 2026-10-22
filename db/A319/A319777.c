/*
 * A319777  a(n) is the number of equivalence classes of triples of sets each
 *          with n or fewer elements where two triples are equivalent if the
 *          number of elements in all intersections is the same.
 * https://oeis.org/A319777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n‑1) - 20*a(n‑2) + 28*a(n‑3) - 14*a(n‑4) - 14*a(n‑5)
 *            + 28*a(n‑6) - 20*a(n‑7) + 7*a(n‑8) - a(n‑9) for n > 8 Initial
 *            values (n = 0 … 8) a(0)=1, a(1)=15, a(2)=100, a(3)=436,
 *            a(4)=1459, a(5)=4069, a(6)=9929, a(7)=21871, a(8)=44426
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 51, u64 n <= 1277,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array of computed terms      */
static size_t cache_cap = 0;     /* current allocated capacity   */

/* Grow cache to at least 'needed' elements */
static void ensure_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute a(n) for any n >= 0 */
seq_t a(idx_t n)
{
    size_t need = (size_t)n + 1;
    ensure_cap(need);

    /* base values (they are defined for all n) */
    cache[0] = (seq_t)1;
    if (need > 1) cache[1] = (seq_t)15;
    if (need > 2) cache[2] = (seq_t)100;
    if (need > 3) cache[3] = (seq_t)436;
    if (need > 4) cache[4] = (seq_t)1459;
    if (need > 5) cache[5] = (seq_t)4069;
    if (need > 6) cache[6] = (seq_t)9929;
    if (need > 7) cache[7] = (seq_t)21871;
    if (need > 8) cache[8] = (seq_t)44426;

    /* compute remaining terms if needed */
    for (size_t i = 9; i <= n; ++i) {
        seq_t v =  7 * cache[i-1]
                 -20 * cache[i-2]
                 +28 * cache[i-3]
                 -14 * cache[i-4]
                 -14 * cache[i-5]
                 +28 * cache[i-6]
                 -20 * cache[i-7]
                 + 7 * cache[i-8]
                 -  cache[i-9];
        cache[i] = v;
    }

    return cache[(size_t)n];
}
