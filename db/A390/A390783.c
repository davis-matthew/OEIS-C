/*
 * A390783  Least k > a(n-1) such that k + the sum of all previous terms = 0
 *          (mod (k-2)), with a(1)=1 and a(2)=2.
 * https://oeis.org/A390783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A390783:
   a(1)=1, a(2)=2. For n>2, let S=Σ_{i=1}^{n-1} a(i) and G=a(n-1)-2.
   Find the smallest divisor D of (S+2) that is > G; then a(n)=D+2.
   Equivalent: a(n) is the least k>a(n-1) with (S+k) mod (k-2)=0. */

#include <stdlib.h>
#include <stddef.h>

static seq_t *a_cache   = (seq_t *)0;
static seq_t *sum_cache = (seq_t *)0;
static size_t cache_cap = 0;          /* allocated capacity of the arrays */
static size_t cache_size = 0;         /* number of terms currently stored (>=2 after init) */

static void ensure_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
    while (new_cap < need) new_cap <<= 1;
    a_cache   = (seq_t *)realloc(a_cache,   new_cap * sizeof(seq_t));
    sum_cache = (seq_t *)realloc(sum_cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

static void init_bases(void)
{
    if (cache_size >= 2) return;          /* already initialized */
    ensure_cache(2);
    a_cache[0]   = (seq_t)1;    /* a(1) */
    sum_cache[0] = (seq_t)1;    /* sum up to a(1) */
    a_cache[1]   = (seq_t)2;    /* a(2) */
    sum_cache[1] = (seq_t)3;    /* sum up to a(2) */
    cache_size = 2;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but guard against misuse */

    size_t target = (size_t)n;
    init_bases();

    if (target <= cache_size) return a_cache[target-1];

    /* compute terms up to target, extending the cache as necessary */
    for (size_t i = cache_size + 1; i <= target; ++i) {
        /* previous term a(i-1) and its sum S */
        seq_t prev   = a_cache[i-2];
        seq_t sum    = sum_cache[i-2];
        /* G = a(i-1)-2, D must divide S+2 */
        seq_t G = prev - (seq_t)2;
        seq_t D = sum + (seq_t)2;

        /* search for the smallest divisor of D that is > G */
        seq_t t = G + (seq_t)1;
        for (;;) {
            if ((D % t) == (seq_t)0) break;
            ++t;
        }

        seq_t new_a   = t + (seq_t)2;          /* a(i) = D+2 */
        seq_t new_sum = sum + new_a;          /* cumulative sum up to a(i) */

        /* ensure storage for the new term */
        if (cache_size >= cache_cap) {
            ensure_cache(cache_size + 1);
        }
        a_cache[cache_size]   = new_a;
        sum_cache[cache_size] = new_sum;
        cache_size++;
    }

    return a_cache[target-1];
}
