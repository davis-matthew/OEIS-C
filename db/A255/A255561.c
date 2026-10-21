/*
 * A255561  Numbers whose binary representation traces a non-selfcrossing
 *          circuit in the honeycomb lattice when each one of its bits, from
 *          the most significant to the least significant, is interpreted as a
 *          direction to proceed at each vertex.
 * https://oeis.org/A255561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 172, u32 n <= 9531, u64 n <= 9531,
 *            u128 n <= 9531, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes a(n) by enumerating integers x and testing if the binary word of x
   corresponds to a closed non-self-crossing circuit on the honeycomb lattice.
   The result is memoised in a static cache.
*/

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Number of cache entries to preallocate – enough for n up to ~20000. */
#define CACHE_SIZE (20000)

/* Direction vectors for the axial honeycomb coordinate system.  */
static const int32_t dir_dx[6] = { 1,  0, -1, -1,  0,  1 };
static const int32_t dir_dy[6] = { 0,  1,  1,  0, -1, -1 };

/* Test whether a 64-bit integer represents a valid word. */
static bool is_valid_honeycomb(uint64_t v)
{
    if (v == 0) /* special empty path */
        return true;

    /* Find most significant bit index.  */
    int msb = 63 - __builtin_clzll(v);

    /* Initial position and direction (east). */
    int32_t x = 0, y = 0;
    int dir = 0;

    /* Store visited vertices – primal visited array (max 65 vertices). */
    struct { int32_t x, y; } visited[66];
    size_t visited_len = 1;
    visited[0].x = 0;
    visited[0].y = 0;

    for (int i = msb; i >= 0; --i) {
        bool bit = (v >> i) & 1ULL;
        /* Turn right = -1, left = +1 in axial coordinate system. */
        dir = (dir + (bit ? 5 : 1)) % 6;   /* +5 ≡ -1 mod 6 */

        x += dir_dx[dir];
        y += dir_dy[dir];

        if (i == 0) {   /* final step */
            return (x == 0 && y == 0);
        }

        /* Early return if we return to the start before the last step. */
        if (x == 0 && y == 0)
            return false;

        /* Check if the new vertex has been visited before. */
        for (size_t j = 0; j < visited_len; ++j) {
            if (visited[j].x == x && visited[j].y == y)
                return false;
        }

        visited[visited_len].x = x;
        visited[visited_len].y = y;
        ++visited_len;
    }
    return false; /* should never reach here */
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    static seq_t cache[CACHE_SIZE];
    static size_t cache_len = 0;          /* number of filled entries */
    static uint64_t last_candidate = 0;   /* last integer examined */

    /* Initialise cache on first call. */
    if (cache_len == 0) {
        cache[0] = (seq_t)0;
        cache_len = 1;
        last_candidate = 0;
    }

    /* Extend cache until we have at least n+1 terms. */
    while (cache_len <= (size_t)n) {
        uint64_t v = last_candidate + 1;
        last_candidate = v;

        if (is_valid_honeycomb(v)) {
            if (cache_len < CACHE_SIZE)
                cache[cache_len++] = (seq_t)v;
            else
                /* Should not happen for expected n. */
                return (seq_t)v;
        }
    }

    return cache[n];
}
