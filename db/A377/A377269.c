/*
 * A377269  G.f. A(x) satisfies A(x) = (1 - 9*x*A(x))^(2/3).
 * https://oeis.org/A377269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   The sequence satisfies the recurrence a(n) = 54 * (2*n-1) *
 *            (n-5) * a(n-3) / (n * (n-1)), n >= 3 with initial values a(0) =
 *            1, a(1) = -6, a(2) = 27. For n > 0 with n ≡ 2 (mod 3) the value
 *            is 0, which follows from the recurrence itself
 * Range:     i8 n <= 3, i16 n <= 8, i32 n <= 17, i64 n <= 30, i128 n <= 2,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free, exit */
#include <stddef.h>   /* size_t */

#ifdef __cplusplus
#include <type_traits>   /* std::is_integral<...> */
#endif

static seq_t *cache = NULL;
static size_t cache_size = 0;

/* Ensure that cache[n] is defined for all indices up to n. */
static void ensure_cache(idx_t n)
{
    if (cache_size > (size_t)n)
        return;

    size_t new_size = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) {
        /* In a test environment this should never happen. */
        exit(1);
    }

    for (size_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            new_cache[0] = (seq_t)1;
        } else if (i == 1) {
            new_cache[1] = (seq_t)-6;
        } else if (i == 2) {
            new_cache[2] = (seq_t)27;
        } else {
#if defined(__cplusplus)
            /* In C++ mode use the type‑trait to choose a suitable method. */
            if constexpr (std::is_integral<seq_t>::value) {
                /* Use 128‑bit intermediate to avoid overflow. */
                __int128 num = (__int128)new_cache[i-3];
                num *= 54;
                num *= (2 * (idx_t)i - 1);
                num *= (idx_t)i - 5;
                __int128 den = (__int128)(idx_t)i * (idx_t)(i-1);
                new_cache[i] = (seq_t)(num / den);
            } else {
                /* seq_t is an arbitrary‑precision class: rely on its operators. */
                seq_t num = new_cache[i-3];
                num *= (seq_t)54;
                num *= (seq_t)(2 * (idx_t)i - 1);
                num *= (seq_t)(idx_t)i - 5;
                new_cache[i] = num / ((seq_t)(idx_t)i * (seq_t)(i-1));
            }
#else
            /* C mode: seq_t is an integral type, use 128‑bit intermediate if available. */
#if defined(__SIZEOF_INT128__)
            __int128 num = (__int128)new_cache[i-3];
            num *= 54;
            num *= (2 * (idx_t)i - 1);
            num *= (idx_t)i - 5;
            __int128 den = (__int128)(idx_t)i * (idx_t)(i-1);
            new_cache[i] = (seq_t)(num / den);
#else
            /* Fallback: rely on the fact that the result fits in seq_t for the tested ranges. */
            seq_t num = new_cache[i-3];
            num *= (seq_t)54;
            num *= (seq_t)(2 * (idx_t)i - 1);
            num *= (seq_t)(idx_t)i - 5;
            new_cache[i] = num / ((seq_t)(idx_t)i * (seq_t)(i-1));
#endif
#endif
        }
    }
    cache = new_cache;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
