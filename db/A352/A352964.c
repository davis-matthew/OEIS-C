/*
 * A352964  a(0) = 0, a(1) = 1, and for any n > 1, a(n) = a(n-F(e)) +
 *          a(n-F(e+1)) with e as large as possible (where F(k) is the k-th
 *          Fibonacci number).
 * https://oeis.org/A352964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 9736, u32 n <= 9736, u64 n <= 9736,
 *            u128 n <= 9736, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A352964 -- a(0)=0, a(1)=1,  a(n)=a(n-F(e))+a(n-F(e+1))
    where e is the largest integer with F(e+1) <= n
    (F(k) is the k-th Fibonacci number, F(1)=1,F(2)=1,…).
*/

#include <stdlib.h>
#include <stdint.h>

static seq_t *seq_cache   = NULL;   /* a(n) values */
static size_t seq_cap     = 0;      /* capacity of seq_cache */
static size_t seq_last    = 0;      /* highest n for which seq_cache[n] is valid */

static seq_t *fib_cache   = NULL;   /* Fibonacci numbers */
static size_t fib_cap     = 0;      /* capacity of fib_cache */
static size_t fib_size    = 0;      /* number of Fibonacci numbers stored */

/* Ensure fib_cache contains all Fibonacci numbers <= n plus the first one > n */
static void ensure_fib(idx_t n)
{
    if (fib_cap == 0) {
        fib_cap = 8;
        fib_cache = (seq_t *)malloc(fib_cap * sizeof(seq_t));
        if (!fib_cache) abort();
        fib_cache[0] = (seq_t)0;
        fib_cache[1] = (seq_t)1;
        fib_cache[2] = (seq_t)1;
        fib_size = 3;
    }

    /* Grow the array if needed when appending new Fibonacci numbers */
    while ((idx_t)fib_cache[fib_size - 1] <= n) {
        if (fib_size >= fib_cap) {
            fib_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(fib_cache, fib_cap * sizeof(seq_t));
            if (!tmp) abort();
            fib_cache = tmp;
        }
        fib_cache[fib_size] = fib_cache[fib_size - 1] + fib_cache[fib_size - 2];
        ++fib_size;
    }
}

/* Binary search to find largest index m with fib_cache[m] <= n  (m>=1) */
static size_t find_m(idx_t n)
{
    size_t lo = 1, hi = fib_size - 1, ans = 1;
    while (lo <= hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (fib_cache[mid] <= (seq_t)n) {
            ans = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;          /* safety, should not happen */
            hi = mid - 1;
        }
    }
    return ans;
}

/* Ensure seq_cache[0..n] is filled */
static void ensure_seq(idx_t n)
{
    if ((size_t)n + 1 > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap : 16;
        while ((size_t)n + 1 > new_cap) new_cap <<= 1;
        seq_t *new_seq = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!new_seq) abort();
        seq_cache = new_seq;
        seq_cap = new_cap;
    }

    /* Initialize base cases if not already done */
    if (seq_last < 1) {
        if (seq_cap < 2) {          /* unlikely, but guard */
            seq_t *new_seq = (seq_t *)realloc(seq_cache, 2 * sizeof(seq_t));
            if (!new_seq) abort();
            seq_cache = new_seq;
            seq_cap = 2;
        }
        seq_cache[0] = (seq_t)0;
        seq_cache[1] = (seq_t)1;
        seq_last = 1;
    }

    for (size_t i = seq_last + 1; i <= (size_t)n; ++i) {
        /* Ensure Fibonacci numbers are available for this i */
        ensure_fib((idx_t)i);

        /* Find e such that fib[e+1] <= i < fib[e+2] */
        size_t m = find_m((idx_t)i);
        size_t e = m - 1;                 /* e+1 = m */

        size_t j = i - (size_t)fib_cache[e];
        size_t k = i - (size_t)fib_cache[e + 1];
        seq_cache[i] = seq_cache[j] + seq_cache[k];
    }

    seq_last = (size_t)n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    ensure_fib((idx_t)n);
    ensure_seq((idx_t)n);
    return seq_cache[(size_t)n];
}
