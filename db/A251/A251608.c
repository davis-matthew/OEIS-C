/*
 * A251608  Lexicographically earliest subsequence of the Fibonacci numbers
 *          (cf. A000045), such that gcd(a(n-2), a(n)) > 1 and gcd(a(n-1),
 *          a(n)) = 1.
 * https://oeis.org/A251608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 13, u64 n <= 27, u128 n <= 47,
 *            big any n
 * Helpers:   fib, idx_gcd, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A251608: lexicographically earliest subsequence of Fibonacci numbers
 * such that gcd(a(n-2), a(n)) > 1 and gcd(a(n-1), a(n)) = 1.
 *
 * The terms are generated greedily: starting from
 *   a(1) = F₃ = 2   (index 3)
 *   a(2) = F₄ = 3   (index 4)
 * and for n ≥ 3 the next term is the Fibonacci number F_k with the
 * smallest index k > a(n‑1)'s index that satisfies
 *   gcd(prevIndex2, k) ≥ 3     (so gcd(F_prev2, F_k) > 1)
 *   gcd(prevIndex1, k) ≤ 2     (so gcd(F_prev1, F_k) = 1)
 *
 * The function below caches all computed indices and values, and is
 * independent of the order of calls.  It works for any build of
 * seq_t (8‑, 16‑, 32‑, 64‑bit or arbitrary‑precision). */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/fib.h"          /* fib(idx_t n) -> seq_t */
#include "../utils/seq_alloc.h"    /* seq_alloc(size_t) -> seq_t* */
#include "../utils/seq_realloc.h"  /* seq_realloc(seq_t*, size_t, size_t) -> seq_t* */
#include "../utils/idx_gcd.h"

/* gcd for index type (idx_t) */

/* Cached state, file scope, persists across calls */
static idx_t *idx_arr        = NULL; /* selected Fibonacci indices */
static seq_t *val_arr       = NULL; /* corresponding Fibonacci values */
static size_t cap           = 0;    /* capacity of the arrays */
static size_t sz            = 0;    /* number of terms currently stored */

/* Ensure that the cache contains at least 'need' terms (need ≥ 1). */
static void ensure_up_to(idx_t need)
{
    /* Initialise storage if this is the first call */
    if (cap == 0) {
        cap = 8;
        idx_arr = (idx_t *)malloc(cap * sizeof(idx_t));
        val_arr = seq_alloc(cap);          /* uses constructor for seq_t */
    }

    /* Grow storage if required */
    if (cap < need) {
        size_t new_cap = cap;
        while (new_cap < need) new_cap <<= 1;
        idx_arr = (idx_t *)realloc(idx_arr, new_cap * sizeof(idx_t));
        val_arr = seq_realloc(val_arr, cap, new_cap);
        cap = new_cap;
    }

    /* Insert the first two terms if they are missing */
    if (sz == 0) {
        idx_arr[0] = 3;         /* index of F₃ */
        val_arr[0] = fib(3);    /* F₃ = 2 */
        sz = 1;
    }
    if (sz == 1) {
        idx_arr[1] = 4;         /* index of F₄ */
        val_arr[1] = fib(4);    /* F₄ = 3 */
        sz = 2;
    }

    /* Generate successive terms until the cache contains 'need' terms */
    while (sz < (size_t)need) {
        idx_t prev2 = idx_arr[sz - 2];
        idx_t prev1 = idx_arr[sz - 1];
        idx_t k = prev1 + 1;    /* start searching after prev1 */

        for (;;) {
            if (idx_gcd(prev2, k) < 3) { k++; continue; }
            if (idx_gcd(prev1, k) > 2) { k++; continue; }
            break; /* k satisfies the constraints */
        }

        idx_arr[sz] = k;
        val_arr[sz] = fib(k);
        sz++;
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* n is 1‑based; ensure the cache holds the n‑th term */
    ensure_up_to(n);
    return val_arr[n - 1];
}
