/*
 * A327922  Odd numbers m >= 3 for which phi(2*m)/2 = phi(m)/2 is even, where
 *          phi = A000010 (Euler's totient).
 * https://oeis.org/A327922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth odd integer ≥3 such that φ(m) is divisible by 4
 *            (i.e. 4 | φ(m))
 * Range:     u8 n <= 92, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_cnt = 0;      /* number of terms stored */
    static idx_t last_checked = 3;   /* last odd number that was tested */
    static size_t cache_cap = 0;     /* capacity of cache */

    /* generate terms until we have at least n of them */
    while (cache_cnt < n) {
        /* next odd number */
        last_checked += 2;
        uint64_t m = last_checked;
        int r1 = 0, r3 = 0;

        /* factor m and count distinct primes 1 mod 4 and 3 mod 4 */
        uint64_t rem = m;
        for (uint64_t p = 3; p <= rem / p; p += 2) {
            if (rem % p == 0) {
                int mod = (int)(p % 4);
                while (rem % p == 0) {
                    rem /= p;
                }
                if (mod == 1) {
                    ++r1;
                } else {
                    ++r3;
                }
            }
        }
        if (rem > 1) {            /* remaining prime factor */
            if (rem % 4 == 1) {
                ++r1;
            } else {
                ++r3;
            }
        }

        /* condition 2*r1 + r3 >= 2 */
        if (2 * r1 + r3 >= 2) {
            /* enlarge cache if needed */
            if (cache_cnt == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
#ifdef __cplusplus
                seq_t *new_cache = new seq_t[new_cap];
                for (idx_t i = 0; i < cache_cnt; ++i) {
                    new_cache[i] = cache[i];
                }
                delete [] cache;
                cache = new_cache;
                cache_cap = new_cap;
#else
                cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache_cap = new_cap;
#endif
            }
            cache[cache_cnt++] = (seq_t)m;
        }
    }

    return cache[n - 1];
}
