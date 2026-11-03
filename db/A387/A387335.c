/*
 * A387335  1, 2, 4, along with numbers divisible by at least two distinct
 *          primes.
 * https://oeis.org/A387335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer that is either 1, 2, 4, or has
 *            at least two distinct prime divisors. (OEIS A387335)
 * Range:     u8 n <= 188, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 9733 (2026-11-03)
 * Helpers:   mem, omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/omega.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Static cache: index 0 corresponds to a(1) */
    static seq_t *cache = NULL;
    static size_t cap   = 0;  /* allocated capacity */
    static size_t len   = 0;  /* number of terms computed */
    static idx_t  next  = 1;  /* next integer to test */

    if (n == 0)               /* offset starts at 1; undef for 0 */
        return (seq_t)0;

    while (len < n) {
        seq_t val = (seq_t)next;

        /* Accept 1, 2, 4 or any number with at least two distinct prime factors */
        if (next == 1 || next == 2 || next == 4 ||
            omega((uint64_t)next) >= 2) {
            /* Ensure space for the new term */
            grow_seq(&cache, &cap, len + 1);
            cache[len++] = val;
        }
        ++next;
    }

    return cache[n - 1];
}
