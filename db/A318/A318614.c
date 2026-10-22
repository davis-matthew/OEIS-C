/*
 * A318614  Scaled g.f. S(u) = Sum_{n>0} a(n)*16*(u/16)^n satisfies T(u) =
 *          d/du S(u), with T(u) as defined by A318417; sequence gives a(n).
 * https://oeis.org/A318614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   (n-1)^2 * n * a(n) – 12 * (n-1) * (2n-3)^2 * a(n-1) + 128 *
 *            (n-2) * (2n-5) * (2n-3) * a(n-2) = 0 with a(1) = 1, a(2) = 6
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
#include <type_traits>
#endif

/* Forward declaration of the recurrence helper */
static seq_t compute_term(idx_t i, seq_t prev, seq_t prevprev);

/* Main sequence function */
seq_t a(idx_t n) {
    /* Static cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t  cache_cap = 0;
    static idx_t   cached_up_to = 0;

    if (n <= 2) {
        /* Base cases */
        if (n == 1) return 1;
        return 6; /* n == 2 */
    }

    /* Initialise cache if needed */
    if (!cache) {
        cache_cap = 4; /* at least indices 0..3 */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 0;        /* unused */
        cache[1] = 1;
        cache[2] = 6;
        cache[3] = 0;        /* placeholder */
        cached_up_to = 2;
    }

    /* Grow cache to hold index n */
    if ((size_t)(n + 1) > cache_cap) {
        size_t needed = n + 1;
        while (cache_cap < needed) cache_cap <<= 1;
        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* Compute forward only; cache keeps the farthest computed index */
    for (idx_t i = cached_up_to + 1; i <= n; ++i) {
        cache[i] = compute_term(i, cache[i-1], cache[i-2]);
    }
    cached_up_to = n;

    return cache[n];
}

/* Recurrence helper implemented differently for C and C++ builds
   and for integral vs. arbitrary-precision seq_t types. */
static seq_t compute_term(idx_t i, seq_t prev, seq_t prevprev) {
#if defined(__cplusplus)
    /* C++ build: decide at compile time whether seq_t is an integral type */
    if constexpr (std::is_integral<seq_t>::value) {
        /* Integral seq_t – use 128‑bit to avoid overflow */
        unsigned long long m1 =
            12ULL * (i - 1) * (2 * i - 3) * (2 * i - 3);
        unsigned long long m2 =
            128ULL * (i - 2) * (2 * i - 5) * (2 * i - 3);

        unsigned __int128 num1 = (unsigned __int128)prev * m1;
        unsigned __int128 num2 = (unsigned __int128)prevprev * m2;
        unsigned __int128 numer = num1 - num2;
        unsigned __int128 denom =
            (unsigned __int128)(i - 1) * (i - 1) * (unsigned int)i;

        return (seq_t)(numer / denom);
    } else {
        /* Arbitrary-precision seq_t – pure seq_t arithmetic is safe */
        unsigned long long m1 =
            12ULL * (i - 1) * (2 * i - 3) * (2 * i - 3);
        unsigned long long m2 =
            128ULL * (i - 2) * (2 * i - 5) * (2 * i - 3);

        seq_t mult1 = (seq_t)m1;
        seq_t mult2 = (seq_t)m2;

        seq_t t1 = prev * mult1;
        seq_t t2 = prevprev * mult2;
        seq_t numer = t1 - t2;

        seq_t denom = (seq_t)((i - 1) * (i - 1) * (unsigned int)i);
        return numer / denom;
    }
#else
    /* C build: seq_t is an integer type – use 128‑bit intermediate */
    unsigned long long m1 =
        12ULL * (i - 1) * (2 * i - 3) * (2 * i - 3);
    unsigned long long m2 =
        128ULL * (i - 2) * (2 * i - 5) * (2 * i - 3);

    unsigned __int128 num1 = (unsigned __int128)prev * m1;
    unsigned __int128 num2 = (unsigned __int128)prevprev * m2;
    unsigned __int128 numer = num1 - num2;
    unsigned __int128 denom =
        (unsigned __int128)(i - 1) * (i - 1) * (unsigned int)i;

    return (seq_t)(numer / denom);
#endif
}
