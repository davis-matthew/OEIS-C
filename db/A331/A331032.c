/*
 * A331032  Number of iterations of n -> n + gpf(n) needed for the trajectory
 *          of n to join the trajectory of A076271, where gpf(n) is the
 *          greatest prime factor of n.
 * https://oeis.org/A331032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 262, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A331032 – Number of iterations of n → n + gpf(n) needed for the
 *  trajectory of n to join the trajectory of A076271 (starting at 1).
 *  The computation is performed by building the orbit of A076271 on
 *  demand and simulating the orbit of n until the first intersection
 *  is found.
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gpf.h"

static idx_t *orbit   = NULL;  /* increasing list of orbit values      */
static size_t orb_size = 0;     /* number of elements stored            */
static size_t orb_cap   = 0;     /* allocated capacity                   */

/* Expand the orbit of A076271 until its last element is ≥ limit. */
static void ensure_orbit_upto(idx_t limit)
{
    /* initial allocation */
    if (orb_cap == 0) {
        orb_cap = 64;
        orbit   = (idx_t *)malloc(orb_cap * sizeof(idx_t));
        if (orbit == NULL) abort();
        orbit[0] = 1;
        orb_size = 1;
    }

    /* grow the orbit as needed */
    while (orbit[orb_size - 1] < limit) {
        idx_t next = orbit[orb_size - 1] + gpf(orbit[orb_size - 1]);

        if (orb_size == orb_cap) {
            size_t new_cap = orb_cap * 2;
            idx_t *tmp = (idx_t *)realloc(orbit, new_cap * sizeof(idx_t));
            if (tmp == NULL) abort();
            orbit = tmp;
            orb_cap = new_cap;
        }
        orbit[orb_size++] = next;
    }
}

/* Binary search to test whether x lies on the A076271 orbit. */
static bool on_orbit(idx_t x)
{
    size_t lo = 0;
    size_t hi = orb_size;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (orbit[mid] < x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (lo < orb_size && orbit[lo] == x);
}

/*  Sequence a(n).  */
seq_t a(idx_t n)
{
    seq_t steps = 0;
    idx_t cur   = n;

    ensure_orbit_upto(cur);
    if (on_orbit(cur))
        return steps;

    for (;;) {
        cur += gpf(cur);        /* one iteration of the rule        */
        ++steps;
        ensure_orbit_upto(cur);
        if (on_orbit(cur))
            return steps;
    }
}
