/*
 * A384707  Consecutive states of the linear congruential pseudo-random number
 *          generator 71971110957370*s mod (2^47-115) when started at s=1.
 * https://oeis.org/A384707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 71971110957370 * a(n-1) mod (2^47-115); a(1)=1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#ifdef __cplusplus
#include <type_traits>
#endif

/* Constants.  The values are small enough to fit in 64 bits.  */
static const seq_t MUL_CONST = (seq_t)71971110957370ULL;
static const seq_t MOD_CONST = (seq_t)(140737488355328ULL - 115ULL); /* 2^47 − 115 */

/* Helper for the modular multiplication.
   In C++ we use a constexpr branch to keep 128‑bit intermediates for
   integral types; arbitrary‑precision types fall back to ordinary
   arithmetic.  In C we rely on the __int128 type if available. */
#ifdef __cplusplus
inline seq_t mult_mod(seq_t a, seq_t b, seq_t mod)
{
    if constexpr (std::is_integral_v<seq_t>) {
        unsigned __int128 prod = (unsigned __int128)a * (unsigned __int128)b;
        return (seq_t)(prod % (unsigned __int128)mod);
    } else {
        return (a * b) % mod;
    }
}
#else
static inline seq_t mult_mod(seq_t a, seq_t b, seq_t mod)
{
#if defined(__SIZEOF_INT128__)
    unsigned __int128 prod = (unsigned __int128)a * (unsigned __int128)b;
    return (seq_t)(prod % (unsigned __int128)mod);
#else
    return (a * b) % mod;
#endif
}
#endif

/* Static cache for memoisation.  cache[0] is unused. */
static seq_t *cache = NULL;
static idx_t last_computed = 0;

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    if (n > last_computed) {
        /* Grow the cache to accommodate index n. */
        idx_t new_size = n + 1;               /* indices 0 … n inclusive */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;                  /* allocation failure: return 0 */

        cache = tmp;

        /* Initialise the first entry if this is the first call. */
        if (last_computed == 0) {
            cache[1] = (seq_t)1;
            last_computed = 1;
        }

        /* Compute consecutive terms up to n. */
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            cache[i] = mult_mod(prev, MUL_CONST, MOD_CONST);
        }
        last_computed = n;
    }

    return cache[n];
}
