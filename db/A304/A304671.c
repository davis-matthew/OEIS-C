/*
 * A304671  Number of nX3 0..1 arrays with every element unequal to 0, 2, 3, 5
 *          or 6 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 44, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 * Empirical recurrence (for n > 19):
 * a(n) = 4*a(n-1) -8*a(n-3) -13*a(n-4) +20*a(n-5) +21*a(n-6) +4*a(n-7)
 *        -30*a(n-8) -44*a(n-9) -17*a(n-10) +72*a(n-11) +116*a(n-12)
 *        -20*a(n-13) -72*a(n-14) -52*a(n-15) -12*a(n-16) +24*a(n-17) +8*a(n-18)
 * Initial values:
 * a(1)=1, a(2)=7, a(3)=13, a(4)=23, a(5)=45, a(6)=131, a(7)=337,
 * a(8)=883, a(9)=2389, a(10)=6599, a(11)=18393, a(12)=51475,
 * a(13)=144777, a(14)=408787, a(15)=1156245, a(16)=3275327,
 * a(17)=9287881, a(18)=26355499, a(19)=74822993
 */
#include <stdlib.h>

static const seq_t init[19] = {
    1ull, 7ull, 13ull, 23ull, 45ull, 131ull, 337ull, 883ull,
    2389ull, 6599ull, 18393ull, 51475ull, 144777ull,
    408787ull, 1156245ull, 3275327ull, 9287881ull, 26355499ull,
    74822993ull
};

#ifdef __cplusplus
/* In C++ mode seq_t may be a big integer class */
static inline seq_t mul_const(seq_t a, unsigned int c) {
    return a * (seq_t)c;
}
#else
/* In C mode seq_t is an unsigned integral type */
static inline seq_t mul_const(seq_t a, unsigned int c) {
    return (seq_t)((unsigned __int128)a * c);
}
#endif

seq_t a(idx_t n) {
    /* Since n is 1-based, return 0 for n<=0 (undefined but harmless) */
    if (n == 0) return (seq_t)0;

    /* Static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap   = 0;

    /* Initialize cache with initial 19 terms */
    if (!cache) {
        cache_cap   = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        for (size_t i = 0; i < 19; ++i)
            cache[i] = init[i];
        cache_size = 19;
    }

    /* Extend cache up to requested n */
    while (cache_size < (size_t)n) {
        size_t idx = cache_size;          /* idx == number of terms stored == n-1 */

        /* Ensure capacity */
        if (cache_size >= cache_cap) {
            size_t new_cap = cache_cap << 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }

#ifdef __cplusplus
        /* Use seq_t intermediate (big integer) */
        seq_t res = mul_const(cache[idx - 1], 4);
        res -= mul_const(cache[idx - 3], 8);
        res -= mul_const(cache[idx - 4], 13);
        res += mul_const(cache[idx - 5], 20);
        res += mul_const(cache[idx - 6], 21);
        res += mul_const(cache[idx - 7], 4);
        res -= mul_const(cache[idx - 8], 30);
        res -= mul_const(cache[idx - 9], 44);
        res -= mul_const(cache[idx -10], 17);
        res += mul_const(cache[idx -11], 72);
        res += mul_const(cache[idx -12], 116);
        res -= mul_const(cache[idx -13], 20);
        res -= mul_const(cache[idx -14], 72);
        res -= mul_const(cache[idx -15], 52);
        res -= mul_const(cache[idx -16], 12);
        res += mul_const(cache[idx -17], 24);
        res += mul_const(cache[idx -18], 8);
#else
        /* Use unsigned __int128 for intermediate to avoid overflow */
        unsigned __int128 res = 0;
        res += (unsigned __int128)cache[idx - 1] * 4;
        res -= (unsigned __int128)cache[idx - 3] * 8;
        res -= (unsigned __int128)cache[idx - 4] * 13;
        res += (unsigned __int128)cache[idx - 5] * 20;
        res += (unsigned __int128)cache[idx - 6] * 21;
        res += (unsigned __int128)cache[idx - 7] * 4;
        res -= (unsigned __int128)cache[idx - 8] * 30;
        res -= (unsigned __int128)cache[idx - 9] * 44;
        res -= (unsigned __int128)cache[idx -10] * 17;
        res += (unsigned __int128)cache[idx -11] * 72;
        res += (unsigned __int128)cache[idx -12] * 116;
        res -= (unsigned __int128)cache[idx -13] * 20;
        res -= (unsigned __int128)cache[idx -14] * 72;
        res -= (unsigned __int128)cache[idx -15] * 52;
        res -= (unsigned __int128)cache[idx -16] * 12;
        res += (unsigned __int128)cache[idx -17] * 24;
        res += (unsigned __int128)cache[idx -18] * 8;
        cache[cache_size++] = (seq_t)res;
        continue;
#endif
        cache[cache_size++] = res;
    }

    return cache[(size_t)n - 1];
}
