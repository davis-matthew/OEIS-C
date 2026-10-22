/*
 * A385955  a(0) = 1; a(n) = Sum_{k=0..n-1} binomial(k+6,6) * a(k) * a(n-1-k).
 * https://oeis.org/A385955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1
   a(n) = Σ_{k=0}^{n-1} binomial(k+6,6) * a(k) * a(n-1-k)   for n ≥ 1
   (OEIS A385955)
*/

#include <stdlib.h>   /* for malloc, realloc, abort */
#include <stdint.h>   /* for uint64_t */

static seq_t *cache = NULL;        /* memoisation array, grows lazily   */
static size_t cap = 0;             /* current capacity of `cache` */

/* compute C(k+6,6) : product (k+6)(k+5)...(k+1) / 720.
   The division is exact for all non‑negative k. */
static inline seq_t binom6(idx_t k)
{
    seq_t r = 1;
    for (idx_t i = 1; i <= 6; ++i)
        r *= (seq_t)(k + 6 - (i - 1));   /* multiply by k+6, k+5, …, k+1 */
    r /= (seq_t)720;                    /* 6! = 720, division is exact */
    return r;
}

/* ensure that the memoisation array can hold index `n` */
static void ensure_cap(idx_t n)
{
    size_t new_cap = (size_t)n + 1;
    if (new_cap > cap) {
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* out of memory – abort the test harness */
            abort();
        }
        cache = tmp;
        /* initialise any newly allocated elements to zero */
        for (size_t i = cap; i < new_cap; ++i)
            cache[i] = 0;
        cap = new_cap;
    }
}

/* public function: returns the n‑th element of the sequence. */
seq_t a(idx_t n)
{
    ensure_cap(n);

    /* initialise a(0) once */
    if (cache[0] == 0)
        cache[0] = 1;

    /* compute any missing values up to n (in any call order) */
    for (idx_t i = 1; i <= n; ++i) {
        if (cache[i] != 0)
            continue;          /* already computed */

        seq_t sum = 0;
        for (idx_t k = 0; k < i; ++k) {
            seq_t term = binom6(k) * cache[k] * cache[i - 1 - k];
            sum += term;
        }
        cache[i] = sum;
    }

    return cache[n];
}
