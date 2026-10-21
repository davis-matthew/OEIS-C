/*
 * A298023  Partial sums of A298022.
 * https://oeis.org/A298023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 167, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A298023: partial sums of A298022.
   Recurrence with non‑homogeneous term at n=8:
   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - 2*a(n-4) + a(n-5)   (n>=6, n≠8)
   a(8) = -2 + 2*a(7) - a(6) + a(5) - 2*a(4) + a(3)
   Initial values:
   a(0)=1, a(1)=4, a(2)=11, a(3)=23, a(4)=40, a(5)=63
   The function is memoised so calls can be in any order. ---*/

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;       /* cache of computed terms   */
static size_t cap = 0;            /* current allocated size    */
static idx_t computed_upto = 0;   /* highest index computed    */

/* Ensure cache holds at least (min_cap+1) elements.  */
static void expand_capacity(idx_t min_cap)
{
    if ((size_t)min_cap < cap)
        return;
    size_t new_cap = (cap == 0) ? 8 : cap;
    while (new_cap <= (size_t)min_cap)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        abort(); /* out of memory */
    if (cache) {
        for (size_t i = 0; i < cap; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }
    cache = new_cache;
    cap = new_cap;
}

/* Initialise the first six terms.  */
static void init_base(void)
{
    if (computed_upto >= 5)
        return;
    expand_capacity(5);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)4;
    cache[2] = (seq_t)11;
    cache[3] = (seq_t)23;
    cache[4] = (seq_t)40;
    cache[5] = (seq_t)63;
    computed_upto = 5;
}

/* Compute all terms up to n.  */
static void compute_to(idx_t n)
{
    init_base();
    if (n <= computed_upto)
        return;
    expand_capacity(n);
    for (idx_t i = computed_upto + 1; i <= n; ++i) {
        if (i == 8) {
            /* special non‑homogeneous case */
            seq_t pos = 2 * cache[i - 1] + cache[i - 3] + cache[i - 5];
            seq_t neg = cache[i - 2] + 2 * cache[i - 4] + (seq_t)2;
            cache[i] = pos - neg;
        } else {
            /* homogeneous recurrence */
            seq_t pos = 2 * cache[i - 1] + cache[i - 3] + cache[i - 5];
            seq_t neg = cache[i - 2] + 2 * cache[i - 4];
            cache[i] = pos - neg;
        }
    }
    computed_upto = n;
}

/* Return the n-th term of A298023.  */
seq_t a(idx_t n)
{
    compute_to(n);
    return cache[n];
}
