/*
 * A281207  Number of 3 X n 0..1 arrays with no element equal to more than one
 *          of its horizontal and antidiagonal neighbors, with the exception
 *          of exactly one element, and with new values introduced in order 0
 *          sequentially upwards.
 * https://oeis.org/A281207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence (n > 8) * a(n) = 3*a(n-1) - a(n-2) -
 *            3*a(n-3) + a(n-4) + a(n-5) * Initial values: * a(1)=0, a(2)=10,
 *            a(3)=56, a(4)=98, * a(5)=176, a(6)=310, a(7)=537, a(8)=922 * *
 *            This implementation memoises the sequence so calls may arrive in
 *            any * order. It works for both C11 and C++17: *...
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 38, u64 n <= 82, u128 n <= 173,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, calloc, exit */
#include <stddef.h>   /* NULL, size_t */

static seq_t *cache = NULL;      /* memoisation array, index 1‑based */
static idx_t  cache_size = 0;    /* allocated size (including index 0)   */
static idx_t  computed   = 0;    /* largest n for which a(n) is stored   */

/* Ensure we have space for indices up to n (inclusive).  */
static void ensure_cache_size(idx_t n)
{
    if (cache_size <= n) {
        idx_t new_size = n + 1;                     /* 1‑based + dummy 0 */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(1);                         /* allocation failed */
        cache      = tmp;
        cache_size = new_size;
    }
}

/* Initialise the first eight terms.  It is safe to call repeatedly.  */
static void initialise_base(void)
{
    cache[1] = (seq_t)0;
    cache[2] = (seq_t)10;
    cache[3] = (seq_t)56;
    cache[4] = (seq_t)98;
    cache[5] = (seq_t)176;
    cache[6] = (seq_t)310;
    cache[7] = (seq_t)537;
    cache[8] = (seq_t)922;
    if (computed < 8) computed = 8;
}

/* Compute the sequence up to n, inclusive.  */
static void compute_up_to(idx_t n)
{
    if (n < 1) return;

    /* First time: allocate at least enough for the base values */
    if (!cache) {
        idx_t alloc = (n > 8) ? n : 8;
        cache = (seq_t *)calloc(alloc + 1, sizeof(seq_t)); /* 0‑based dummy */
        if (!cache) exit(1);
        cache_size = alloc + 1;
        initialise_base();
    } else {
        ensure_cache_size(n);
    }

    /* Make sure the base terms are present */
    if (computed < 8) initialise_base();

    /* Start computing from the first undefined term */
    idx_t start = (computed < 9) ? 9 : computed + 1;
    for (idx_t i = start; i <= n; ++i) {
        seq_t term = (seq_t)3 * cache[i-1]
                     - cache[i-2]
                     - (seq_t)3 * cache[i-3]
                     + cache[i-4]
                     + cache[i-5];
        cache[i] = term;
    }
    if (computed < n) computed = n;
}

/* Public function required by the testing harness */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */
    compute_up_to(n);
    return cache[n];
}
