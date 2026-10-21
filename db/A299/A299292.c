/*
 * A299292  Partial sums of A299291.
 * https://oeis.org/A299292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + a(n-3) - a(n-5) + 2*a(n-6) - 2*a(n-8) - 2*a(n-9)
 *            + 2*a(n-11) - a(n-12) + a(n-14) + a(n-15) - a(n-17) for n>20
 *            Initial values: a(0)=1, a(1)=6, a(2)=20, a(3)=49, a(4)=105,
 *            a(5)=190, a(6)=320, a(7)=501, a(8)=727, a(9)=1026, a(10)=1408,
 *            a(11)=1853, a(12)=2391, a(13)=3026, a(14)=3734,...
 * Range:     u8 n <= 5, u16 n <= 36, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* cached sequence values          */
static size_t capacity = 0;          /* allocated array size            */
static size_t computed_len = 0;      /* first index not yet computed    */

/* Base terms a(0) .. a(20) */
static const seq_t base_vals[21] = {
    1,   6,   20,   49,  105,  190,  320,  501,
  727, 1026, 1408, 1853, 2391, 3026, 3734, 4579,
 5541, 6620, 7838, 9201,10657
};

static void ensure_capacity(size_t needed)
{
    if (capacity < needed) {
        /* grow in chunks to reduce realloc frequency */
        size_t new_cap = capacity ? capacity * 2 : 64;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        capacity = new_cap;
    }
    if (computed_len == 0) {
        /* first-time initialization of base values */
        for (size_t i = 0; i < 21; ++i) cache[i] = base_vals[i];
        computed_len = 21;  /* indices 0..20 are now valid */
    }
}

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    /* guarantee storage */
    ensure_capacity(N + 1);
    if (N < computed_len) return cache[N];

    /* compute values from first missing index up to N using recurrence */
    for (size_t i = computed_len; i <= N; ++i) {
        /* recurrence valid for i >= 21 */
        cache[i] = cache[i-2] + cache[i-3] - cache[i-5]
                 + 2*cache[i-6] - 2*cache[i-8] - 2*cache[i-9]
                 + 2*cache[i-11] - cache[i-12] + cache[i-14]
                 + cache[i-15] - cache[i-17];
    }
    computed_len = N + 1;
    return cache[N];
}
