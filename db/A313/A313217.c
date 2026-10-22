/*
 * A313217  Coordination sequence Gal.6.382.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 4, a(2) = 9, a(3) = 15, a(4) = 19, a(5) = 25,
 *            a(6) = 32, a(7) = 35, a(8) = 40, a(9) = 46, a(10) = 50, a(11) =
 *            54, a(12) = 62, a(13) = 67, a(14) = 69, a(15) = 76, a(16) = 81,
 *            a(17) = 84, a(18) = 91, a(19) = 99. For n > 19: a(n) = 2*a(n-1)
 *            - 3*a(n-2) + 4*a(n-3) - 5*a(n-4) + 5*a(n-5...
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t computed_up_to = 0;

/* Ensure *cache has at least (n+1) slots */
static void ensure_cache_capacity(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (cache_cap < need) {
        size_t new_cap = cache_cap < 16 ? 16 : cache_cap;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* First time: allocate cache, fill known initials */
    if (!cache) {
        size_t init_cap = 32;
        cache = (seq_t *)calloc(init_cap, sizeof(seq_t));
        if (!cache) abort();
        cache_cap = init_cap;

        /* initial 20 values (required to seed the recurrence) */
        static const seq_t base[20] = {
            1, 4, 9, 15, 19, 25, 32, 35, 40, 46,
            50, 54, 62, 67, 69, 76, 81, 84, 91, 99
        };
        for (size_t i = 0; i < 20; ++i) cache[i] = base[i];
        computed_up_to = 19;
    }

    if (n <= (idx_t)computed_up_to) return cache[(size_t)n];

    ensure_cache_capacity(n);

    for (idx_t i = (idx_t)computed_up_to + 1; i <= n; ++i) {
        seq_t val = 0;
        val +=  2 * cache[(size_t)(i - 1)];
        val -=  3 * cache[(size_t)(i - 2)];
        val +=  4 * cache[(size_t)(i - 3)];
        val -=  5 * cache[(size_t)(i - 4)];
        val +=  5 * cache[(size_t)(i - 5)];
        val -=  4 * cache[(size_t)(i - 6)];
        val +=  4 * cache[(size_t)(i - 7)];
        val -=  4 * cache[(size_t)(i - 8)];
        val +=  4 * cache[(size_t)(i - 9)];
        val -=  4 * cache[(size_t)(i -10)];
        val +=  5 * cache[(size_t)(i -11)];
        val -=  5 * cache[(size_t)(i -12)];
        val +=  4 * cache[(size_t)(i -13)];
        val -=  3 * cache[(size_t)(i -14)];
        val +=  2 * cache[(size_t)(i -15)];
        val -=  1 * cache[(size_t)(i -16)];
        cache[(size_t)i] = val;
    }

    computed_up_to = (size_t)n;
    return cache[(size_t)n];
}
