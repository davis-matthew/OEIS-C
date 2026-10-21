/*
 * A278603  A prime mountain: peaks and valleys beyond the origin correspond
 *          to prime abscissa (see Comments for precise definition).
 * https://oeis.org/A278603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5936, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(0)=0, a(1)=1.
   For n>=2 let d_n = a(n)-a(n-1).  
   Then d_n = d_{n-1}           if n-1 is composite,
         d_n = -d_{n-1}          if n-1 is prime.
   Hence a(n)=a(n-1)+d_n.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --- helper primitives ------------------------------------ */

static bool is_prime_local(idx_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    for (idx_t i = 5; i <= n / i; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

/* --- cached state ---------------------------------------- */

static seq_t *seq_cache   = NULL;      /* array of computed a(n)  */
static size_t seq_cap     = 0;         /* allocated capacity      */
static idx_t   seq_last    = 0;         /* highest index stored    */
static seq_t   diff_last   = 0;         /* last difference d_n     */

/* ensure cache has space for indices up to 'n' inclusive */
static void ensure_cache_capacity(idx_t n)
{
    if (seq_cap > (size_t)n) return;
    size_t new_cap = seq_cap ? seq_cap : 2;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq_cache = tmp;
    seq_cap   = new_cap;
}

/* first construction of the cache */
static void init_cache(void)
{
    if (seq_cache) return;
    seq_cap = 2;
    seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    if (!seq_cache) abort();
    seq_cache[0] = (seq_t)0;
    seq_cache[1] = (seq_t)1;
    seq_last     = 1;
    diff_last    = (seq_t)1;          /* a(1)-a(0) */
}

/* extend cache up to index 'n' (inclusive) */
static void compute_up_to(idx_t n)
{
    if (n <= seq_last) return;
    ensure_cache_capacity(n);
    for (idx_t i = seq_last + 1; i <= n; ++i) {
        /* i corresponds to a(i) */
        bool prime = is_prime_local(i - 1);   /* test for i-1 */
        if (prime) diff_last = -diff_last;    /* flip sign */
        /* otherwise keep diff_last unchanged */
        seq_cache[i] = seq_cache[i-1] + diff_last;
    }
    seq_last = n;
}

/* public interface ---------------------------------------- */

seq_t a(idx_t n)
{
    init_cache();
    if (n <= seq_last) return seq_cache[n];
    compute_up_to(n);
    return seq_cache[n];
}
