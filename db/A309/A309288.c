/*
 * A309288  a(0) = 0, a(1) = 1, and for any n > 1, a(n) = Sum_{k > 1} (-1)^k *
 *          a(floor(n/k)).
 * https://oeis.org/A309288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = Sum_{k>1} (-1)^k * a(floor(n/k))
 * Range:     i8 n <= 1023, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static seq_t *cache   = NULL;
static bool *visited  = NULL;
static idx_t  cap     = 0;

/* Internal memoised computation of a(n) */
static seq_t a_internal(idx_t n)
{
    /* Resize cache and visited arrays if needed */
    if (n >= cap) {
        idx_t new_cap = n + 1;
        cache    = (seq_t *)realloc(cache,    new_cap * sizeof(seq_t));
        visited  = (bool *)realloc(visited,  new_cap * sizeof(bool));
        /* Initialise new entries as unvisited */
        for (idx_t i = cap; i < new_cap; ++i)
            visited[i] = false;
        cap = new_cap;
    }

    if (visited[n])
        return cache[n];

    if (n == 0) {
        cache[0] = (seq_t)0;
        visited[0] = true;
        return cache[0];
    }
    if (n == 1) {
        cache[1] = (seq_t)1;
        visited[1] = true;
        return cache[1];
    }

    /* Use a wider integer type for intermediate sums to avoid overflow.
       __int128 is available on GCC/Clang for 64‑bit seq_t builds.  If seq_t is
       already wider, the cast is safe. */
    __int128 sum = 0;

    for (idx_t k = 2; k <= n; ++k) {
        seq_t sub = a_internal(n / k);
        if (k & 1)          /* odd k -> (-1) */
            sum -= (__int128)sub;
        else                /* even k -> (+1) */
            sum += (__int128)sub;
    }

    seq_t result = (seq_t)sum;
    cache[n] = result;
    visited[n] = true;
    return result;
}

/* Public interface required by the build */
seq_t a(idx_t n)
{
    return a_internal(n);
}
