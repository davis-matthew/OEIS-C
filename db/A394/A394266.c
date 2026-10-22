/*
 * A394266  Number of induced cubes in the n-odd graph.
 * https://oeis.org/A394266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1)=1, a(2)=6, a(3)=25;
   (-25*n^2+19*n+12)*a(n) + 18*(2*n-1)*(n-2)*a(n-1) + 4*(n-1)*(n+1)*a(n+1) = 0
   For n > 3, a(n+1) = ((25*n^2-19*n-12)*a(n) - 18*(2*n-1)*(n-2)*a(n-1)) / (4*(n-1)*(n+1))
*/

#include <stdlib.h>
#include <stddef.h>
#include <string.h>

/* static cache for memoized values */
static seq_t *cache = NULL;
/* number of allocated slots (indices 0 .. capacity-1) */
static size_t capacity = 0;
/* highest index computed in cache (0 if none) */
static idx_t computed_max = 0;

/* ensure that indices up to n are allocated */
static void ensure_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1u;               /* indices 0 .. n inclusive */
    if (needed <= capacity)
        return;
    /* allocate new buffer and copy existing data */
    seq_t *buf = (seq_t*)calloc(needed, sizeof(seq_t));
    if (!buf)
        exit(EXIT_FAILURE);
    if (cache) {
        memcpy(buf, cache, (size_t)(computed_max + 1u) * sizeof(seq_t));
        free(cache);
    }
    cache = buf;
    capacity = needed;
}

/* initialise base values a(1), a(2), a(3) iff not already present */
static void init_base(void)
{
    if (computed_max >= 3)
        return;
    /* need space for indices 0..3 */
    if (capacity < 4u)
        ensure_capacity(3);
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)6;
    cache[3] = (seq_t)25;
    computed_max = 3;
}

/* sequence a(n) for n >= 1 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n==0 */
    init_base();
    ensure_capacity(n);
    if (n <= computed_max)
        return cache[n];
    /* compute values up to n */
    for (idx_t i = computed_max + 1u; i <= n; ++i) {
        /* recurrence uses a(i-1) and a(i-2) to produce a(i)  */
        idx_t prev = i - 1u;
        idx_t prevprev = i - 2u;
        seq_t a_prev = cache[prev];
        seq_t a_prevprev = cache[prevprev];
        /* numerator: (25*prev^2 - 19*prev -12)*a_prev
                     - 18*(2*prev-1)*(prev-2)*a_prevprev */
        seq_t num = ((seq_t)25 * (seq_t)prev * (seq_t)prev
                     - (seq_t)19 * (seq_t)prev
                     - (seq_t)12) * a_prev
                    - ((seq_t)18 * (seq_t)(2u * prev - 1u)
                       * (seq_t)(prev - 2u) * a_prevprev);
        seq_t denom = (seq_t)4u * (seq_t)(prev - 1u) * (seq_t)(prev + 1u);
        cache[i] = num / denom;
    }
    computed_max = n;
    return cache[n];
}
