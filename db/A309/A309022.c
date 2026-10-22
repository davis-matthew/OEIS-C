/*
 * A309022  Expansion of x * Product_{k>=0} (1 - x^(2^k) - x^(2^(k+1)) -
 *          x^(2^(k+2))).
 * https://oeis.org/A309022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 90, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A309022:  a(0)=0, a(1)=1;
   a(2n)= -a(n),  a(2n+1)= a(n+1)-a(n)-a(n-1)   (n≥1)  */
#include <stdlib.h>
#include <stdbool.h>

/* ---------- internal memoisation structures ---------- */
static seq_t *cache   = NULL;   /* array of computed values      */
static bool *computed = NULL;   /* true if cache[i] is valid     */
static size_t cap     = 0;      /* current capacity of arrays    */

/* Ensure that cache[0..n] exists and has been initialised */
static void
ensure_capacity(idx_t n)
{
    if (cap > n) return;
    size_t new_cap = (cap == 0) ? (n + 1) : cap;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    /* resize cache array */
    if (cache != NULL) {
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    } else {
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    }
    /* resize computed flag array */
    if (computed != NULL) {
        computed = (bool *)realloc(computed, new_cap * sizeof(bool));
    } else {
        computed = (bool *)malloc(new_cap * sizeof(bool));
    }
    /* initialise flags for new elements */
    for (size_t i = cap; i < new_cap; ++i) computed[i] = false;
    cap = new_cap;
}

/* Recursive computation with memoisation */
static seq_t
rec(idx_t n)
{
    if (computed[n]) return cache[n];

    seq_t val;
    if (n == 0) {
        val = 0;
    } else if (n == 1) {
        val = 1;
    } else if ((n & 1) == 0) {              /* n even  */
        val = -rec(n >> 1);
    } else {                                 /* n odd   */
        idx_t k = n >> 1;                    /* floor(n/2) */
        val = rec(k + 1) - rec(k) - rec(k - 1);
    }
    cache[n]   = val;
    computed[n] = true;
    return val;
}

/* ---------- public API ------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    ensure_capacity(n);
    return rec(n);
}
