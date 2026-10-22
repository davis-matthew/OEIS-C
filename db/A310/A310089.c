/*
 * A310089  Coordination sequence Gal.6.42.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-5) - a(n-10)  for n > 10
   Initial values: a(0)=1, a(1)=3, a(2)=6, a(3)=11, a(4)=13,
                    a(5)=16, a(6)=19, a(7)=22, a(8)=29, a(9)=29, a(10)=32 */

#include <stddef.h> /* for size_t */

/* Cache large enough for all n used in tests (≈10000). */
static seq_t cache[20001];
static idx_t used = 0;

static void init_cache(void)
{
    cache[0]  = (seq_t)1;
    cache[1]  = (seq_t)3;
    cache[2]  = (seq_t)6;
    cache[3]  = (seq_t)11;
    cache[4]  = (seq_t)13;
    cache[5]  = (seq_t)16;
    cache[6]  = (seq_t)19;
    cache[7]  = (seq_t)22;
    cache[8]  = (seq_t)29;
    cache[9]  = (seq_t)29;
    cache[10] = (seq_t)32;
    used = 11;
}

seq_t a(idx_t n)
{
    if (used == 0)
        init_cache();

    if (n < used)
        return cache[n];

    /* Compute missing entries up to n */
    for (idx_t i = used; i <= n; ++i)
    {
        cache[i] = (seq_t)2 * cache[i - 5] - cache[i - 10];
    }
    used = n + 1;
    return cache[n];
}
