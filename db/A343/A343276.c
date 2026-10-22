/*
 * A343276  a(n) = n! * [x^n] -x*(x + 1)*exp(x)/(x - 1)^3.
 * https://oeis.org/A343276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k^2 * k! * binomial(n,k) Recurrence: a(0)=0,
 *            a(1)=1, a(2)=10, a(n) = ((2 + n*(n+2))*a(n-1))/(n-1) -
 *            (n+1)*a(n-2) for n >= 3
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#if defined(__cplusplus)
#include <type_traits>

/* Multiplication with overflow protection for integral types */
template<typename T>
inline typename std::enable_if<std::is_integral<T>::value, T>::type
safe_mul_int(T a, T b) {
    unsigned __int128 temp = (unsigned __int128)a * b;
    return (T)temp;
}
template<typename T>
inline typename std::enable_if<!std::is_integral<T>::value, T>::type
safe_mul_int(T a, T b) {
    return a * b;
}

/* Multiplication and division with overflow protection for integral types */
template<typename T>
inline typename std::enable_if<std::is_integral<T>::value, T>::type
safe_mul_div(T a, T b, T denom) {
    unsigned __int128 temp = (unsigned __int128)a * b;
    return (T)(temp / denom);
}
template<typename T>
inline typename std::enable_if<!std::is_integral<T>::value, T>::type
safe_mul_div(T a, T b, T denom) {
    return (a * b) / denom;
}
#else
/* For C, assume seq_t is an unsigned integral type */
static inline seq_t safe_mul_int(seq_t a, seq_t b) {
    unsigned __int128 temp = (unsigned __int128)a * b;
    return (seq_t)temp;
}
static inline seq_t safe_mul_div(seq_t a, seq_t b, seq_t denom) {
    unsigned __int128 temp = (unsigned __int128)a * b;
    return (seq_t)(temp / denom);
}
#endif

/* Static cache for memoization */
static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t max_computed = 0;

/* Compute a(n) */
seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* Initialize cache if needed */
    if (cache == NULL) {
        cache_size = 2;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 1;
        max_computed = 1;
    }

    /* Ensure cache has enough space */
    if (n >= cache_size) {
        size_t newsize = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, newsize * sizeof(seq_t));
        cache = tmp;
        cache_size = newsize;
    }

    /* Compute missing terms */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            if (i == 2) {
                cache[i] = 10;
                continue;
            }
            /* factor = 2 + i*(i+2) */
            seq_t factor = (seq_t)i * ((seq_t)i + 2) + 2;
            /* term = factor * a(i-1) / (i-1) */
            seq_t term = safe_mul_div(factor, cache[i-1], (seq_t)(i-1));
            /* subtract (i+1)*a(i-2) */
            seq_t sub = safe_mul_int(((seq_t)i + 1), cache[i-2]);
            term -= sub;
            cache[i] = term;
        }
        max_computed = n;
    }

    return cache[n];
}
