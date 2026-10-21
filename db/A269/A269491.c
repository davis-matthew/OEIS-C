/*
 * A269491  Number of length-n 0..5 arrays with no repeated value differing
 *          from the previous repeated value by one.
 * https://oeis.org/A269491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A269491
   Number of length‑n 0..5 arrays with no repeated value differing from
   the previous repeated value by one.

   Recurrence (for n ≥ 8):
       a(n) = 20*a(n-1) - 145*a(n-2) + 425*a(n-3) - 252*a(n-4)
              - 605*a(n-5) - 53*a(n-6) + 15*a(n-7)
   Initial terms:
       a(1) =   6, a(2) =  36, a(3) = 216, a(4) = 1286,
       a(5) = 7616, a(6) = 44904, a(7) = 263794.  */

#include <stdlib.h>
#ifdef __cplusplus
#include <type_traits>
#endif

/* Cached values: cache[0] = a(1) */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Pre‑computed seed values for a(1) … a(7) */
static const seq_t seed_vals[7] = {
    (seq_t)6,
    (seq_t)36,
    (seq_t)216,
    (seq_t)1286,
    (seq_t)7616,
    (seq_t)44904,
    (seq_t)263794
};

/* Compute a(n) from the stored cache (n≥8).  Works for both integral
   and non‑integral seq_t types. */
static inline seq_t rec_compute(idx_t idx, const seq_t *c)
{
    /* idx is 1‑based, c is the cache array */
#ifdef __cplusplus
    /* If seq_t is an integral type, use __int128 for intermediate
       calculations to avoid overflow.  Otherwise fall back to the
       generic arithmetic provided by the arbitrary‑precision class. */
    if constexpr (std::is_integral<seq_t>::value) {
        idx_t i = idx - 1;
        __int128_t t1 = (__int128_t)c[i-1] * 20;
        __int128_t t2 = (__int128_t)c[i-2] * 145;
        __int128_t t3 = (__int128_t)c[i-3] * 425;
        __int128_t t4 = (__int128_t)c[i-4] * 252;
        __int128_t t5 = (__int128_t)c[i-5] * 605;
        __int128_t t6 = (__int128_t)c[i-6] * 53;
        __int128_t t7 = (__int128_t)c[i-7] * 15;
        __int128_t res = t1 - t2 + t3 - t4 - t5 - t6 + t7;
        return (seq_t)res;
    } else
#endif
    {
        idx_t i = idx - 1;
        seq_t res = (seq_t)20 * c[i-1];
        res -= (seq_t)145 * c[i-2];
        res += (seq_t)425 * c[i-3];
        res -= (seq_t)252 * c[i-4];
        res -= (seq_t)605 * c[i-5];
        res -= (seq_t)53  * c[i-6];
        res += (seq_t)15  * c[i-7];
        return res;
    }
}

/* External linkage function required by the test harness */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* undefined for n = 0 */

    /* Grow cache if needed */
    if (n > cache_len) {
        idx_t new_len = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;             /* allocation failure */
        cache = tmp;
        /* Fill seed values up to 7 (or up to new_len if smaller) */
        for (idx_t i = 0; i < new_len && i < 7; ++i)
            cache[i] = seed_vals[i];
        /* Compute remaining terms */
        for (idx_t i = cache_len; i < new_len; ++i) {
            idx_t idx = i + 1;          /* 1‑based index of a(idx) */
            if (idx <= 7)
                cache[i] = seed_vals[i];  /* already set above */
            else
                cache[i] = rec_compute(idx, cache);
        }
        cache_len = new_len;
    }
    return cache[n - 1];
}
