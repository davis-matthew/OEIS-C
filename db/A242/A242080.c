/*
 * A242080  Length of period of Nim sequence of subtraction game on first n
 *          terms of A242079.
 * https://oeis.org/A242080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 2. For n >= 2: let k = 1 if n is a Fibonacci
 *            number (>= 2), otherwise k = 2. a(n) = 3*a(n-1) - k*a(n-2)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#ifdef __cplusplus
#include <type_traits>
#endif

/* ----------------------------------------------------------------
   Helper to multiply by 3 and by 2, using a wider type where
   necessary to avoid overflow for the plain unsigned integer
   builds.  The fallback for non‑integral types (e.g. arbitrary‑precision
   classes) simply uses the built‑in operator.
   ---------------------------------------------------------------- */
#ifdef __cplusplus
static inline seq_t mul_by_3(seq_t x)
{
    if constexpr (std::is_integral<seq_t>::value)
        return (seq_t)((unsigned __int128)3 * (unsigned __int128)x);
    else
        return (seq_t)3 * x;
}
static inline seq_t mul_by_2(seq_t x)
{
    if constexpr (std::is_integral<seq_t>::value)
        return (seq_t)((unsigned __int128)2 * (unsigned __int128)x);
    else
        return (seq_t)2 * x;
}
#else
static inline seq_t mul_by_3(seq_t x)
{
    return (seq_t)((unsigned __int128)3 * (unsigned __int128)x);
}
static inline seq_t mul_by_2(seq_t x)
{
    return (seq_t)((unsigned __int128)2 * (unsigned __int128)x);
}
#endif

/* ----------------------------------------------------------------
   Fibonacci table: smallest set that contains all Fibonacci
   numbers up to the biggest index that has been requested.
   ---------------------------------------------------------------- */
static uint64_t *fib_tab = NULL;   /* holds Fibonacci numbers */
static size_t   fib_cnt = 0;       /* number of numbers stored */
static size_t   fib_cap = 0;       /* allocated capacity */

/* Ensure that fib_tab contains all Fibonacci numbers <= n. */
static void ensure_fib_up_to(idx_t n)
{
    if (fib_tab == NULL) {
        fib_cap = 32;
        fib_tab = (uint64_t*)malloc(fib_cap * sizeof(uint64_t));
        fib_tab[0] = 1;
        fib_tab[1] = 1;
        fib_cnt = 2;
    }
    while (fib_cnt < fib_cap && fib_tab[fib_cnt-1] < (uint64_t)n) {
        uint64_t next = fib_tab[fib_cnt-1] + fib_tab[fib_cnt-2];
        fib_tab[fib_cnt++] = next;
    }
    while (fib_tab[fib_cnt-1] < (uint64_t)n) {
        if (fib_cap == 0) fib_cap = 32;
        while (fib_cnt == fib_cap) {
            fib_cap *= 2;
            fib_tab = (uint64_t*)realloc(fib_tab, fib_cap * sizeof(uint64_t));
        }
        uint64_t next = fib_tab[fib_cnt-1] + fib_tab[fib_cnt-2];
        fib_tab[fib_cnt++] = next;
    }
}

/* Test whether index n is a Fibonacci number. */
static bool is_fibonacci_idx(idx_t n)
{
    for (size_t i = 0; i < fib_cnt; ++i) {
        if (fib_tab[i] == n) return true;
        if (fib_tab[i] > n) break;
    }
    return false;
}

/* ----------------------------------------------------------------
   Sequence cache: holds a(i) for i up to last_index.
   ---------------------------------------------------------------- */
static seq_t *seq_cache   = NULL;   /* cache */
static idx_t  last_index  = 0;      /* highest index that has been computed */

/* Ensure that seq_cache contains values up to index n. */
static void ensure_cache(idx_t n)
{
    if (n <= last_index) return;

    /* Resize cache if needed */
    if (seq_cache == NULL) {
        size_t init_cap = (n >= 1) ? (size_t)n+1 : 2;
        seq_cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
        if (!seq_cache) exit(1);
        seq_cache[0] = (seq_t)1;
        if (init_cap > 1) seq_cache[1] = (seq_t)2;
    } else if ((size_t)n + 1 > (size_t)last_index + 1) {
        seq_cache = (seq_t*)realloc(seq_cache, ((size_t)n+1) * sizeof(seq_t));
        if (!seq_cache) exit(1);
    }

    /* make sure Fibonacci table is ready */
    ensure_fib_up_to(n);

    /* compute from last_index+1 up to n */
    for (idx_t i = last_index + 1; i <= n; ++i) {
        if (i == 0) { seq_cache[0] = (seq_t)1; continue; }
        if (i == 1) { seq_cache[1] = (seq_t)2; continue; }
        seq_t a_prev1 = seq_cache[i-1];
        seq_t a_prev2 = seq_cache[i-2];
        seq_t val;
        if (is_fibonacci_idx(i)) {
            val = mul_by_3(a_prev1) - a_prev2;   /* k = 1 */
        } else {
            val = mul_by_3(a_prev1) - mul_by_2(a_prev2); /* k = 2 */
        }
        seq_cache[i] = val;
    }
    last_index = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    ensure_cache(n);
    return seq_cache[n];
}
