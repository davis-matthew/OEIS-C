/*
 * A310392  Coordination sequence Gal.6.366.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-6) - a(n-12) for n > 12 * Initial values: a(0)=1,
 *            a(1)=4, a(2)=10, a(3)=14, a(4)=18, a(5)=28, * a(6)=28, a(7)=36,
 *            a(8)=38, a(9)=42, a(10)=46, a(11)=60, a(12)=56
 * Range:     u8 n <= 48, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_CACHE_SIZE 20001U  /* sufficient for n ≤ 10000 */

static const seq_t initial[13] = {
    (seq_t)1,  (seq_t)4,  (seq_t)10, (seq_t)14, (seq_t)18, (seq_t)28,
    (seq_t)28, (seq_t)36, (seq_t)38, (seq_t)42, (seq_t)46, (seq_t)60,
    (seq_t)56
};

static seq_t cache[MAX_CACHE_SIZE];
static bool cache_initialized = false;

seq_t a(idx_t n)
{
    /* Handle the initial segment directly */
    if (n < 13U) {
        return initial[(idx_t)n];
    }

    /* First time we use the cache, copy the known initial values */
    if (!cache_initialized) {
        for (int i = 0; i < 13; ++i) {
            cache[i] = initial[i];
        }
        cache_initialized = true;
    }

    /* n is guaranteed in tests to be ≤ 10000, but we guard anyway */
    size_t idx = (size_t)n;
    if (idx >= MAX_CACHE_SIZE) {
        abort();  /* should not happen in the test harness */
    }

    /* Compute all missing values up to idx */
    for (size_t i = 13; i <= idx; ++i) {
        cache[i] = (seq_t)2 * cache[i - 6U] - cache[i - 12U];
    }

    return cache[idx];
}
