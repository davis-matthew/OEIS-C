/*
 * A276295  Number of nX4 0..2 arrays with no element equal to any value at
 *          offset (-2,-1) (-1,1) or (0,-1) and new values introduced in order
 *          0..2.
 * https://oeis.org/A276295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) -20*a(n-2) +17*a(n-3) -65*a(n-4) +116*a(n-5)
 *            -40*a(n-6) +124*a(n-7) -172*a(n-8) +16*a(n-9) -48*a(n-10)
 *            +64*a(n-11), for n > 13, with a(1)=4, a(2)=36, a(3)=173,
 *            a(4)=1017, a(5)=5965, a(6)=34865, a(7)=203933, a(8)=1192701,
 *            a(9)=6974781, a(10)=40786925, a(11)=238509373, a(12)=1394719...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t base_vals[14] = {
    0ULL,         /* base_vals[0] unused */
    4ULL, 36ULL, 173ULL, 1017ULL, 5965ULL, 34865ULL,
    203933ULL, 1192701ULL, 6974781ULL, 40786925ULL,
    238509373ULL, 1394719389ULL, 8155807421ULL
};

static seq_t *cache = NULL;
static idx_t cache_cap = 0;

static void ensure_cache(idx_t n)
{
    if (n <= cache_cap) return;          /* already computed */

    /* extend cache array */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) abort();              /* allocation failed */
    cache = new_cache;

    /* initialize base values for indices 1..13 if not already present */
    idx_t start = cache_cap + 1;
    if (start <= 13) {
        if (n > 13) n = n; /* keep original n */
        idx_t end = 13;
        if (n < 13) end = n;
        for (idx_t i = start; i <= end; ++i) {
            cache[i] = base_vals[i];
        }
        start = 14;                     /* next index after base initialization */
    }

    /* recurrence for indices i >= 14 */
    if (n >= 14) {
        if (start < 14) start = 14;      /* safety */
        for (idx_t i = start; i <= n; ++i) {
            /* Compute positive part */
            seq_t pos = 0;
            pos += cache[i-1] * (seq_t)9;
            pos += cache[i-3] * (seq_t)17;
            pos += cache[i-5] * (seq_t)116;
            pos += cache[i-7] * (seq_t)124;
            pos += cache[i-9] * (seq_t)16;
            pos += cache[i-11] * (seq_t)64;

            /* Compute negative part */
            seq_t neg = 0;
            neg += cache[i-2] * (seq_t)20;
            neg += cache[i-4] * (seq_t)65;
            neg += cache[i-6] * (seq_t)40;
            neg += cache[i-8] * (seq_t)172;
            neg += cache[i-10] * (seq_t)48;

            /* a(i) = pos - neg; pos >= neg for all n >= 14 */
            cache[i] = pos - neg;
        }
    }

    cache_cap = n;                        /* mark up to n computed */
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                  /* offset is 1; behavior below offset not required */
    ensure_cache(n);
    return cache[n];
}
