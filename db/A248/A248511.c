/*
 * A248511  Difference between k and the least prime factor of k^2+1 where k
 *          is the n-th number with k^2+1 composite.
 * https://oeis.org/A248511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k - spf(k²+1) , where k is the n‑th positive integer such
 *            that k²+1 is composite. spf(x) is the smallest prime divisor of
 *            x (x itself if x is prime)
 * Range:     u8 n <= 215, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Simple trial‑division least prime factor.
   Works for 64‑bit n; for the sizes needed in this sequence it is more than fast enough. */
static uint64_t _least_prime_factor(uint64_t n) {
    if (n % 2 == 0) return 2;
    for (uint64_t d = 3; d * d <= n; d += 2) {
        if (n % d == 0) return d;
    }
    return n;              /* n is prime */
}

/* Grow cache array if necessary. */
static void _ensure_capacity(size_t needed, size_t *cap, seq_t **array) {
    if (needed <= *cap) return;
    size_t newcap = (*cap) ? (*cap) << 1 : 16;
    if (newcap < needed) newcap = needed;
    seq_t *tmp = (seq_t *)realloc(*array, newcap * sizeof(seq_t));
    if (!tmp) abort();     /* terminate on allocation failure */
    *array = tmp;
    *cap = newcap;
}

/* Compute a(n). */
seq_t a(idx_t n) {
    /* Memoisation state (static to survive multiple calls) */
    static seq_t *cache = NULL;
    static idx_t cached = 0;       /* number of terms stored in cache */
    static size_t cap = 0;         /* current cache capacity */
    static idx_t last_k = 1;       /* last k processed; starts before the first possible k (2) */

    if (cached < n) {               /* need to compute more terms */
        _ensure_capacity(n, &cap, &cache);

        for (; cached < n; ++last_k) {
            uint64_t k64 = (uint64_t)last_k;
            uint64_t val = k64 * k64 + 1;              /* k² + 1 */
            uint64_t lpf = _least_prime_factor(val);   /* least prime factor */

            if (lpf != val) {                         /* val is composite */
                cache[cached] = (seq_t)(k64 - lpf);
                ++cached;
            }
        }
    }
    return cache[n - 1];   /* cache is 0‑based, n is 1‑based */
}
