/*
 * A350130  Integers m such that iterating the map f(x) = x^2 + 1 on m
 *          generates a number ending with m.
 * https://oeis.org/A350130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = m such that iterating f(x)=x²+1 six times yields a number
 *            * ending with m (i.e. f⁶(m) ≡ m (mod 10^d) where d is the number
 *            of decimal * digits of m). The function constructs the sequence
 *            digit by digit, * keeping a sorted list of all known terms
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#ifdef __cplusplus
#include <type_traits>
#endif

/* Dynamic array of seq_t values */
typedef struct {
    seq_t *data;
    size_t size;
    size_t cap;
} SeqArray;

/* Ensure the array can hold at least 'needed' elements */
static inline void ensure_capacity(SeqArray *arr, size_t needed)
{
    if (needed > arr->cap) {
        size_t newcap = arr->cap ? arr->cap * 2 : 16;
        while (newcap < needed) newcap <<= 1;
        seq_t *newdata = (seq_t *)realloc(arr->data, newcap * sizeof(seq_t));
        if (!newdata) abort();
        arr->data = newdata;
        arr->cap = newcap;
    }
}

/* Append a value to the array, resizing if necessary */
static inline void push_back(SeqArray *arr, seq_t val)
{
    if (arr->size == arr->cap) {
        size_t newcap = arr->cap ? arr->cap * 2 : 16;
        seq_t *newdata = (seq_t *)realloc(arr->data, newcap * sizeof(seq_t));
        if (!newdata) abort();
        arr->data = newdata;
        arr->cap = newcap;
    }
    arr->data[arr->size++] = val;
}

/* Multiplication modulo 'mod'.  For integral types we use 128‑bit arithmetic
 * to avoid overflow.  For arbitrary‑precision types the overloaded operators
 * handle the multiplication correctly. */
static inline seq_t mulmod(seq_t a, seq_t b, seq_t mod)
{
#ifdef __cplusplus
    if constexpr (std::is_arithmetic<seq_t>::value) {
        return (((unsigned __int128)a * (unsigned __int128)b) % mod);
    } else {
        return (a * b) % mod;
    }
#else
    return (((unsigned __int128)a * (unsigned __int128)b) % mod);
#endif
}

/* Check whether m satisfies f⁶(m) ≡ m (mod base) where f(x)=x²+1 */
static inline bool ok(seq_t m, seq_t base)
{
    seq_t x = m;
    for (int i = 0; i < 6; ++i) {
        seq_t sq = mulmod(x, x, base);
        x = sq + 1;
        if (x >= base) x -= base;
    }
    return x == m;
}

/* Comparator for qsort used to sort arrays of seq_t */
static int seq_qsort_cmp(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety placeholder */

    /* Persistent static state */
    static SeqArray cache  = {NULL, 0, 0};   /* all known terms, sorted */
    static SeqArray curr   = {NULL, 0, 0};   /* solutions modulo current base */
    static SeqArray next   = {NULL, 0, 0};   /* temporary buffer for next digit */
    static seq_t base      = (seq_t)10;     /* current base (10^d) */
    static bool initialized = false;

    if (!initialized) {
        initialized = true;
        /* Find 1‑digit solutions */
        for (seq_t r = 0; r < (seq_t)10; ++r) {
            if (ok(r, base)) {
                push_back(&curr, r);
                push_back(&cache, r);
            }
        }
    }

    /* Grow the sequence until we have at least n terms */
    while (cache.size < (size_t)n) {
        seq_t next_base = base * (seq_t)10; /* 10^(d+1) */

        /* Build next digit solutions */
        ensure_capacity(&next, curr.size * 10 + 10);
        next.size = 0;

        for (size_t i = 0; i < curr.size; ++i) {
            seq_t r = curr.data[i];
            for (int k = 0; k < 10; ++k) {
                seq_t cand = r + (seq_t)k * base;
                if (ok(cand, next_base)) {
                    push_back(&next, cand);
                }
            }
        }

        /* Sort and store new terms not already in cache */
        if (next.size > 0) {
            qsort(next.data, next.size, sizeof(seq_t), seq_qsort_cmp);
            for (size_t i = 0; i < next.size; ++i) {
                seq_t v = next.data[i];
                if (v >= base) {               /* new digit added */
                    push_back(&cache, v);
                }
            }
        }

        /* Advance to next digit level */
        base = next_base;
        SeqArray tmp = curr;
        curr = next;
        next = tmp;
    }

    return cache.data[(size_t)n - 1];
}
