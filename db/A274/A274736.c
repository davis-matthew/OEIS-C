/*
 * A274736  G.f. A(x) satisfies: A(x)^2 - 4*A(x)^3 = A(x^2).
 * https://oeis.org/A274736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A274736
 *  G.f. A(x) satisfies: A(x)^2 - 4*A(x)^3 = A(x^2).
 *  The coefficients are computed by the recurrence derived from that
 *  functional equation. */
/*  seq_t and idx_t are provided by the build environment. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <type_traits>
namespace {
    template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    inline T mul_safe(T a, T b) noexcept {
        return (T)((unsigned __int128)a * (unsigned __int128)b);
    }
    template <typename T, typename std::enable_if<!std::is_integral<T>::value, int>::type = 0>
    inline T mul_safe(T a, T b) noexcept {
        return a * b;
    }
}
#else
static inline seq_t mul_safe(seq_t a, seq_t b) {
    return (seq_t)((unsigned __int128)a * (unsigned __int128)b);
}
#endif

/* Dynamically resized caches */
static seq_t *a_cache = NULL;   /* a[1..]   */
static seq_t *t_cache = NULL;   /* T[2..]   where T[m] = sum_{i=1}^{m-1} a[i]*a[m-i] */
static idx_t cache_size = 0;    /* greatest n already computed */
static idx_t capacity = 0;      /* allocated length of caches (>= cache_size+2) */

/* Expand caches to accommodate indices up to needed (includes T up to needed+1) */
static void expand_cache(idx_t needed) {
    /* needed is the target n; we need capacity >= needed+2 to hold a[1..n] and T[2..n+1] */
    if (needed + 2 <= capacity)
        return;
    size_t new_cap = capacity ? capacity * 2 : 4;
    while (new_cap + 2 < (size_t)(needed + 2))
        new_cap *= 2;
    seq_t *new_a = (seq_t *)calloc(new_cap + 1, sizeof(seq_t));
    seq_t *new_t = (seq_t *)calloc(new_cap + 1, sizeof(seq_t));
    if (!new_a || !new_t) abort();
    if (cache_size > 0) {
        for (idx_t i = 1; i <= cache_size; ++i)
            new_a[i] = a_cache[i];
        for (idx_t i = 2; i <= cache_size + 1; ++i)
            new_t[i] = t_cache[i];
    }
    free(a_cache);
    free(t_cache);
    a_cache = new_a;
    t_cache = new_t;
    capacity = new_cap;
}

/* Build sequence up to index n (inclusive). */
static void build_upto(idx_t n) {
    if (n <= cache_size)
        return;

    expand_cache(n);

    /* Initialise from scratch if cache empty */
    if (cache_size == 0) {
        a_cache[1] = (seq_t)1;          /* a(1)=1 */
        t_cache[2] = mul_safe(a_cache[1], a_cache[1]); /* T(2)=a1*a1 */
        cache_size = 1;
    }

    for (idx_t k = cache_size + 1; k <= n; ++k) {
        /* Compute U(k+1) = Σ_{i=1}^{k-1} a[i] * T(k+1-i) */
#ifdef __cplusplus
        if constexpr (std::is_integral<seq_t>::value) {
            unsigned __int128 U128 = 0;
            for (idx_t i = 1; i < k; ++i)
                U128 += (unsigned __int128)a_cache[i] * (unsigned __int128)t_cache[k + 1 - i];
            seq_t U = (seq_t)U128;

            /* delta = a[(k+1)/2] if k+1 even */
            seq_t delta = (((k + 1) & 1) == 0) ? a_cache[(k + 1) / 2] : (seq_t)0;

            unsigned __int128 Tk1_128 = 4 * U128 + (unsigned __int128)delta;
            seq_t Tk1 = (seq_t)Tk1_128;

            /* Compute S = Σ_{i=2}^{k-1} a[i] * a[k+1-i] */
            unsigned __int128 S128 = 0;
            for (idx_t i = 2; i < k; ++i)
                S128 += (unsigned __int128)a_cache[i] * (unsigned __int128)a_cache[k + 1 - i];
            seq_t a_k = (seq_t)((Tk1_128 - S128) / 2);

            a_cache[k] = a_k;
            t_cache[k + 1] = Tk1;
        } else {
            seq_t U = 0;
            for (idx_t i = 1; i < k; ++i)
                U = U + mul_safe(a_cache[i], t_cache[k + 1 - i]);

            seq_t delta = (((k + 1) & 1) == 0) ? a_cache[(k + 1) / 2] : (seq_t)0;
            seq_t Tk1 = 4 * U + delta;

            seq_t S = 0;
            for (idx_t i = 2; i < k; ++i)
                S = S + mul_safe(a_cache[i], a_cache[k + 1 - i]);

            seq_t a_k = (Tk1 - S) / 2;

            a_cache[k] = a_k;
            t_cache[k + 1] = Tk1;
        }
#else
        unsigned __int128 U128 = 0;
        for (idx_t i = 1; i < k; ++i)
            U128 += (unsigned __int128)a_cache[i] * (unsigned __int128)t_cache[k + 1 - i];
        seq_t U = (seq_t)U128;

        seq_t delta = (((k + 1) & 1) == 0) ? a_cache[(k + 1) / 2] : (seq_t)0;
        unsigned __int128 Tk1_128 = 4 * U128 + (unsigned __int128)delta;
        seq_t Tk1 = (seq_t)Tk1_128;

        unsigned __int128 S128 = 0;
        for (idx_t i = 2; i < k; ++i)
            S128 += (unsigned __int128)a_cache[i] * (unsigned __int128)a_cache[k + 1 - i];
        seq_t a_k = (seq_t)((Tk1_128 - S128) / 2);

        a_cache[k] = a_k;
        t_cache[k + 1] = Tk1;
#endif
    }
    cache_size = n;
}

/* Public interface */
seq_t a(idx_t n) {
    if (n < 1)
        return (seq_t)0;               /* undefined for non‑positive indices */

    build_upto(n);
    return a_cache[n];
}
