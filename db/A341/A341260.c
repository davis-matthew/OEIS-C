/*
 * A341260  Wythoff ceiling array; see Comments.
 * https://oeis.org/A341260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = W*(n,k) = F(k+1)*ceil(n*φ) + (n−1)*F(k)
 * Range:     u8 n <= 55, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where F is the Fibonacci sequence (F(0)=0,F(1)=1) and φ = (1+√5)/2. */
/* The terms are indexed with the standard OEIS offset 1. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* --- Fibonacci table ----------------------------------------------- */
static seq_t *fibs   = NULL;   /* Fibonacci numbers, 0‑based: F(0),F(1),... */
static size_t fib_cap = 0;     /* current allocated capacity */

/* Ensure that fibs[0..max_k] are defined. */
static void ensure_fib(size_t max_k)
{
    if (max_k < fib_cap)
        return;

    size_t new_cap = fib_cap ? fib_cap << 1 : 2;
    if (new_cap <= max_k)
        new_cap = max_k + 1;

    seq_t *new_fibs = (seq_t *)realloc(fibs, new_cap * sizeof(seq_t));
    if (!new_fibs)
        return; /* allocation failure – unlikely in test harness */

    if (!fibs) {              /* first allocation: initialise F(0)=0, F(1)=1 */
        new_fibs[0] = (seq_t)0;
        new_fibs[1] = (seq_t)1;
    }

    /* Start computing from the first index not already initialised. */
    size_t start = fib_cap > 1 ? fib_cap : 2;
    for (size_t i = start; i < new_cap; ++i)
        new_fibs[i] = new_fibs[i-1] + new_fibs[i-2];

    fibs   = new_fibs;
    fib_cap = new_cap;
}

/* --- Ceil(n*φ) table ----------------------------------------------- */
static seq_t *ceil_arr = NULL;  /* ceil_arr[i] = ceil(i*φ), i>=1 */
static size_t ceil_cap = 0;     /* current allocated capacity */

/* Integer square‑root of 128‑bit values via binary search. */
static unsigned __int128 floor_sqrt_uint128(unsigned __int128 v)
{
    unsigned __int128 lo = 0, hi = ((unsigned __int128)1 << 64);
    while (lo < hi) {
        unsigned __int128 mid = (lo + hi + 1) >> 1;
        if (mid * mid <= v)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Compute ceil(i*φ) via the identity */
/*   ceil(i*φ) = floor((i + floor(i*√5))/2) + 1  (i>=0). */
static seq_t ceil_phi(idx_t i)
{
    unsigned __int128 ii = (unsigned __int128)i;
    unsigned __int128 val = ii * ii * 5;            /* 5*i² */
    unsigned __int128 s   = floor_sqrt_uint128(val);/* floor(i*√5) */
    unsigned __int128 floor_phi = (ii + s) / 2;
    unsigned __int128 ceil_phi  = floor_phi + 1;
    return (seq_t)ceil_phi;
}

/* Ensure ceil_arr[0..max_r] are defined. */
static void ensure_ceil_arr(idx_t max_r)
{
    if ((idx_t)max_r < (idx_t)ceil_cap)
        return;

    size_t new_cap = ceil_cap ? ceil_cap << 1 : 2;
    if (new_cap <= (size_t)max_r)
        new_cap = (size_t)max_r + 1;

    seq_t *new_arr = (seq_t *)realloc(ceil_arr, new_cap * sizeof(seq_t));
    if (!new_arr)
        return; /* allocation failure – unlikely */

    if (!ceil_arr) {            /* first allocation: index 0 unused */
        new_arr[0] = (seq_t)0;
    }

    for (size_t i = ceil_cap; i < new_cap; ++i)
        new_arr[i] = ceil_phi((idx_t)i);

    ceil_arr = new_arr;
    ceil_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Main sequence function (external linkage).                          */
seq_t a(idx_t n)
{
    /* Find the antidiagonal (s) such that (s-1)*s/2 >= n. */
    idx_t s = 2;
    while ((s - 1) * s / 2 < n)
        ++s;

    /* Number of elements before this diagonal. */
    idx_t prev = (s - 2) * (s - 1) / 2;

    /* Zero‑based offset within the diagonal. */
    idx_t offset = n - prev - 1;

    /* Coordinates in the Wythoff array: row i, column j.  i>=1, j>=1. */
    idx_t i = offset + 1;
    idx_t j = s - i;

    /* Make sure required Fibonacci numbers and ceil values are ready. */
    ensure_fib((size_t)j + 1);
    ensure_ceil_arr(i);

    seq_t ceil_i = ceil_arr[i];

    /* Compute W*(i,j) = F(j+1)*ceil_i + (i-1)*F(j). */
    seq_t term = fibs[j + 1] * ceil_i + (seq_t)(i - 1) * fibs[j];
    return term;
}
