/*
 * A304583  Period 18: repeat 1,8,3,6,5,4,7,2,9,0,9,2,7,4,5,6,3,8.
 * https://oeis.org/A304583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -a(n-1) + a(n-9) + a(n-10), a(1)=1, a(2)=8, …, a(10)=0
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence holds for n > 10. Initial values for n = 1 … 10 are given above. */

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t */
#include <assert.h>
#include <stdlib.h>   /* abort */

/* Maximum n we can handle.  The tests use n up to about 10 000. */
#define MAX_CAP 20000

/* Persistent cache: 1‑based indexing; cache[0] is unused. */
static seq_t cache[MAX_CAP + 1];
static idx_t max_computed = 0;

/* Sequence function */
seq_t a(idx_t n)
{
    assert(n >= 1);                 /* offset is 1 */
    if (n > MAX_CAP)                /* out of bounds – raise fatal error */
        abort();

    /* If the desired value has already been computed, return it. */
    if (n <= max_computed)
        return cache[n];

    /* Seed initial values for indices 1 … 10. */
    static const seq_t init[10] = {1, 8, 3, 6, 5, 4, 7, 2, 9, 0};

    /* Compute all missing values up to n. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 10) {
            cache[i] = init[i - 1];
        } else {
            /* Recurrence: a(i) = -a(i-1) + a(i-9) + a(i-10) */
            /* All terms are small (0…9), so cast to signed 64‑bit for safety. */
            int64_t tmp = (int64_t)cache[i - 9] + (int64_t)cache[i - 10]
                          - (int64_t)cache[i - 1];
            cache[i] = (seq_t)tmp;   /* intermediate fits in a 64‑bit signed integer */
        }
    }

    max_computed = n;
    return cache[n];
}
