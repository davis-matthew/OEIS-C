/*
 * A289004  a(n) = 2*a(n-1) - a(n-2) + 2*a(n-4) - a(n-5) + a(n-7) - a(n-8) -
 *          a(n-10) for n >= 10, where a(0) = 2, a(1) = 4, a(2) = 7, a(3) =
 *          11, a(4) = 18, a(5) = 31, a(6) = 52, a(7) = 89, a(9) = 151, a(9) =
 *          257.
 * https://oeis.org/A289004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 2*a(n-4) - a(n-5) + a(n-7) - a(n-8) -
 *            a(n-10) Initial values: a(0)=2, a(1)=4, a(2)=7, a(3)=11,
 *            a(4)=18, a(5)=31, a(6)=52, a(7)=89, a(8)=151, a(9)=257
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 40, u64 n <= 81, u128 n <= 164,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

static const seq_t init_vals[10] = {
    (seq_t)2,  (seq_t)4,  (seq_t)7,  (seq_t)11,
    (seq_t)18, (seq_t)31, (seq_t)52, (seq_t)89,
    (seq_t)151, (seq_t)257
};

#define MAX_N 10000
static seq_t cache[MAX_N + 1];
static bool cache_ready = false;
static idx_t max_computed = 9;

static void init_cache(void) {
    for (idx_t i = 0; i < 10; ++i) {
        cache[i] = init_vals[i];
    }
    /* Remaining entries stay zero-initialised by static storage */
    cache_ready = true;
}

seq_t a(idx_t n) {
    if (!cache_ready) {
        init_cache();
    }

    if (n < 10) {
        return cache[(size_t)n];
    }

    if (n > MAX_N) {
        /* For indices beyond the static cache we compute on the fly
           using a rolling buffer of the last 10 terms.  This keeps the
           implementation independent of dynamic allocation and works
           for any seq_t type.  */
        seq_t buf[10];
        for (int i = 0; i < 10; ++i) {
            buf[i] = init_vals[i];
        }
        for (idx_t i = 10; i <= n; ++i) {
            seq_t v = 2 * buf[(i - 1) % 10]
                    - buf[(i - 2) % 10]
                    + 2 * buf[(i - 4) % 10]
                    - buf[(i - 5) % 10]
                    + buf[(i - 7) % 10]
                    - buf[(i - 8) % 10]
                    - buf[(i -10) % 10];
            buf[i % 10] = v;
        }
        return buf[n % 10];
    }

    if (n <= max_computed) {
        return cache[(size_t)n];
    }

    /* Compute missing terms up to n and cache them. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t v = 2 * cache[(size_t)(i - 1)]
                - cache[(size_t)(i - 2)]
                + 2 * cache[(size_t)(i - 4)]
                - cache[(size_t)(i - 5)]
                + cache[(size_t)(i - 7)]
                - cache[(size_t)(i - 8)]
                - cache[(size_t)(i -10)];
        cache[(size_t)i] = v;
    }
    max_computed = n;
    return cache[(size_t)n];
}
