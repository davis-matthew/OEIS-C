/*
 * A292322  p-INVERT of (1,0,0,1,0,0,1,0,0,...), where p(S) = 1 - S - S^2 -
 *          S^3.
 * https://oeis.org/A292322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 60, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is defined by the recurrence
   a(0)=1, a(1)=2, a(2)=4, a(3)=8, a(4)=17, a(5)=36,
   a(6)=73, a(7)=152, a(8)=317, a(9)=653
   a(n) = a(n-1)+a(n-2)+4*a(n-3)-2*a(n-4)-a(n-5)
          -3*a(n-6)+a(n-7)+a(n-9)   for n ≥ 10
*/
#include <stdlib.h>

static seq_t *cache = NULL;          /* cached values  */
static idx_t  cache_size = 0;        /* allocated indices 0..cache_size-1 */
static idx_t  computed_upto = 9;     /* largest index that has been computed */

/* first ten terms */
static const seq_t base[10] =
{ 1, 2, 4, 8, 17, 36, 73, 152, 317, 653 };

/* expand the cache to hold indices 0..n; initialise new slots */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;
    size_t new_cap = (size_t)n + 1;
    seq_t *new_data = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (new_data == NULL) { free(cache); exit(1); }
    /* initialise any newly appended slots */
    for (idx_t i = cache_size; i <= n; ++i)
        new_data[i] = (i < 10) ? base[i] : (seq_t)0;
    cache = new_data;
    cache_size = new_cap;
    /* computed_upto remains 9 after first allocation; it never exceeds cache_size-1 */
}

/* compute terms up to n if not already done */
static void compute_up_to(idx_t n)
{
    if (n <= computed_upto) return;   /* already computed */

    idx_t start = (computed_upto + 1 >= 10) ? computed_upto + 1 : 10;
    if (start > n) return;            /* only base terms needed */

    for (idx_t i = start; i <= n; ++i) {
        /* recurrence for n ≥ 10 */
        cache[i] =
            cache[i-1] + cache[i-2] + 4 * cache[i-3]
            - 2 * cache[i-4] - cache[i-5] - 3 * cache[i-6]
            + cache[i-7] + cache[i-9];
    }
    computed_upto = n;
}

/* public function */
seq_t a(idx_t n)
{
    if (n == (idx_t)-1) return (seq_t)0;   /* defensive guard */
    ensure_cache(n);
    compute_up_to(n);
    return cache[n];
}
