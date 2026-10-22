/*
 * A315605  Coordination sequence Gal.6.577.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-2) - 3*a(n-4) + 4*a(n-6) - 3*a(n-8) + 2*a(n-10) -
 *            a(n-12) for n > 15, with initial values
 *            a(0)=1,a(1)=6,a(2)=12,a(3)=14,a(4)=20,
 *            a(5)=24,a(6)=30,a(7)=38,a(8)=44,a(9)=50,a(10)=56,a(11)=52,
 *            a(12)=64,a(13)=60,a(14)=74,a(15)=82
 * Range:     u8 n <= 46, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* initial 16 terms */
static const seq_t init_vals[16] = {
    1, 6, 12, 14, 20, 24, 30, 38,
    44, 50, 56, 52, 64, 60, 74, 82
};

#define MAX_N 20000          /* safe upper bounds for n (≈10000) */

static seq_t cache[MAX_N + 1];
static bool cache_initialized = false;
static idx_t computed_up_to = 15;   /* highest index that is already cached */

/* initialize the cache with the first 16 terms */
static void init_cache(void)
{
    for (size_t i = 0; i < 16; ++i)
        cache[i] = init_vals[i];
    cache_initialized = true;
}

seq_t a(idx_t n)
{
    if (!cache_initialized)
        init_cache();

    if (n <= computed_up_to)
        return cache[n];

    /* compute terms from computed_up_to+1 up to n */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        seq_t pos = 0;                     /* sum of positive terms */
        pos += cache[i-2] * ((seq_t)2);
        pos += cache[i-6] * ((seq_t)4);
        pos += cache[i-10] * ((seq_t)2);

        seq_t neg = 0;                     /* sum of negative terms */
        neg += cache[i-4] * ((seq_t)3);
        neg += cache[i-8] * ((seq_t)3);
        neg += cache[i-12];                /* coefficient 1 */

        cache[i] = pos - neg;              /* pos ≥ neg by definition */
    }
    computed_up_to = n;
    return cache[n];
}
