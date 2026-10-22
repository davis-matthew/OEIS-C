/*
 * A309512  Sum of the odd parts in the partitions of n into 3 parts.
 * https://oeis.org/A309512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 116, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A309512 – Sum of the odd parts in the partitions of n into 3 parts.
    Recurrence (n > 18):
        a(n) = a(n-1) - a(n-2) + a(n-3) + a(n-4) - a(n-5)
               + 3*a(n-6) - 3*a(n-7) + 2*a(n-8) - 2*a(n-9) - 2*a(n-10)
               + 2*a(n-11) - 3*a(n-12) + 3*a(n-13) - a(n-14) + a(n-15)
               + a(n-16) - a(n-17) + a(n-18) - a(n-19)
    Initial values:
        a(0)=0, a(1)=0, a(2)=0, a(3)=3, a(4)=2, a(5)=6, a(6)=6,
        a(7)=18, a(8)=18, a(9)=37, a(10)=34, a(11)=64, a(12)=64,
        a(13)=100, a(14)=100, a(15)=161, a(16)=156, a(17)=220,
        a(18)=220, a(19)=310
*/

#include <stdlib.h>

static const seq_t base[20] = {
    0ULL, 0ULL, 0ULL, 3ULL, 2ULL, 6ULL, 6ULL, 18ULL, 18ULL, 37ULL,
    34ULL, 64ULL, 64ULL, 100ULL, 100ULL, 161ULL, 156ULL, 220ULL,
    220ULL, 310ULL
};

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t computed = 19;   /* largest n whose a(n) is stored in cache */

/* Allocate (or enlarge) the cache to hold indices up to n. */
static void ensure_cache(idx_t n) {
    if (n < cache_size) return;
    idx_t new_size = cache_size ? cache_size << 1 : 32;
    while (new_size <= n) new_size <<= 1;
#ifdef __cplusplus
    seq_t *tmp = reinterpret_cast<seq_t*>(realloc(cache, new_size * sizeof(seq_t)));
#else
    seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
#endif
    if (!tmp) return;              /* allocation failed; caller will handle */
    cache = tmp;
    for (idx_t i = cache_size; i < new_size; ++i) cache[i] = 0;
    cache_size = new_size;
}

seq_t a(idx_t n) {
    if (n < 20) return base[n];
    if (!cache) {
        ensure_cache(20);
        for (idx_t i = 0; i < 20; ++i) cache[i] = base[i];
        computed = 19;
    }
    if (n > computed) {
        ensure_cache(n);
        for (idx_t k = computed + 1; k <= n; ++k) {
            seq_t s = 0;
            s += cache[k-1];
            s -= cache[k-2];
            s += cache[k-3];
            s += cache[k-4];
            s -= cache[k-5];
            s += 3 * cache[k-6];
            s -= 3 * cache[k-7];
            s += 2 * cache[k-8];
            s -= 2 * cache[k-9];
            s -= 2 * cache[k-10];
            s += 2 * cache[k-11];
            s -= 3 * cache[k-12];
            s += 3 * cache[k-13];
            s -= cache[k-14];
            s += cache[k-15];
            s += cache[k-16];
            s -= cache[k-17];
            s += cache[k-18];
            s -= cache[k-19];
            cache[k] = s;
        }
        computed = n;
    }
    return cache[n];
}
