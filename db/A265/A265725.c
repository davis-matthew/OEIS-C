/*
 * A265725  Number of binary strings of length n having at least one run of
 *          length at least 4.
 * https://oeis.org/A265725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n - 2*A000073(n+2) where A000073 is the Tribonacci
 *            sequence (T(0)=0,T(1)=0,T(2)=1)
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* ------------------------------------------------------------ */
/* Dynamic array helpers that work in both C and C++             */
/* ------------------------------------------------------------ */

#if defined(__cplusplus)
static inline seq_t* alloc_seq_array(size_t n) {
    return new seq_t[n];
}
static inline void grow_seq_array(seq_t **arr, size_t *cap, size_t need) {
    if (*cap >= need) return;
    size_t new_cap = *cap ? *cap * 2 : 64;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = new seq_t[new_cap];
    for (size_t i = 0; i < *cap; ++i) tmp[i] = (*arr)[i];
    delete[] *arr;
    *arr = tmp;
    *cap = new_cap;
}
#else
static inline seq_t* alloc_seq_array(size_t n) {
    return (seq_t*)malloc(n * sizeof(seq_t));
}
static inline void grow_seq_array(seq_t **arr, size_t *cap, size_t need) {
    if (*cap >= need) return;
    size_t new_cap = *cap ? *cap * 2 : 64;
    while (new_cap < need) new_cap *= 2;
    *arr = (seq_t*)realloc(*arr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}
#endif

/* ------------------------------------------------------------ */
/* Caches for 2^n and Tribonacci numbers                       */
/* ------------------------------------------------------------ */
static seq_t *pow2_cache   = NULL;
static size_t pow2_cap     = 0;
static idx_t pow2_last     = 0;      /* last index that has a valid value */

static seq_t *trib_cache   = NULL;
static size_t trib_cap     = 0;
static idx_t trib_last     = 0;      /* last index that has a valid value */

/* ------------------------------------------------------------ */
/* Initialise caches (one‑time only)                           */
/* ------------------------------------------------------------ */
static void init_caches(void) {
    if (pow2_cache == NULL) {
        grow_seq_array(&pow2_cache, &pow2_cap, 64);
        pow2_cache[0] = (seq_t)1;    /* 2^0 = 1 */
        pow2_last = 0;
    }
    if (trib_cache == NULL) {
        grow_seq_array(&trib_cache, &trib_cap, 64);
        trib_cache[0] = (seq_t)0;    /* T(0) = 0 */
        trib_cache[1] = (seq_t)0;    /* T(1) = 0 */
        trib_cache[2] = (seq_t)1;    /* T(2) = 1 */
        trib_last   = 2;
    }
}

/* ------------------------------------------------------------ */
/* Ensure pow2_cache(i) is computed for all i <= n              */
/* ------------------------------------------------------------ */
static void ensure_pow2(idx_t n) {
    if (pow2_last >= n) return;
    grow_seq_array(&pow2_cache, &pow2_cap, n + 1);
    for (idx_t i = pow2_last + 1; i <= n; ++i) {
        pow2_cache[i] = pow2_cache[i - 1];
        pow2_cache[i] = pow2_cache[i] + pow2_cache[i]; /* multiply by 2 */
    }
    pow2_last = n;
}

/* ------------------------------------------------------------ */
/* Ensure trib_cache(i) is computed for all i <= n              */
/* ------------------------------------------------------------ */
static void ensure_trib(idx_t n) {
    if (trib_last >= n) return;
    grow_seq_array(&trib_cache, &trib_cap, n + 1);
    for (idx_t i = trib_last + 1; i <= n; ++i) {
        trib_cache[i] = trib_cache[i - 1];
        trib_cache[i] = trib_cache[i] + trib_cache[i - 2];
        trib_cache[i] = trib_cache[i] + trib_cache[i - 3];
    }
    trib_last = n;
}

/* ------------------------------------------------------------ */
/* Main sequence function                                       */
/* ------------------------------------------------------------ */
seq_t a(idx_t n) {
    /* For small n the answer is zero by definition */
    if (n < 4) return (seq_t)0;

    init_caches();

    /* 2^n */
    ensure_pow2(n);
    seq_t pow2n = pow2_cache[n];

    /* 2 * T(n+2) */
    ensure_trib(n + 2);
    seq_t t = trib_cache[n + 2];
    seq_t twice_t = t + t;

    /* a(n) = 2^n - 2*T(n+2) */
    return pow2n - twice_t;
}
