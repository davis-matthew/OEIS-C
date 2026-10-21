/*
 * A266363  Least positive integer x such that n + x^2 = y^3 + z^4 for some
 *          positive integers y and z, or 0 if no such x exists.
 * https://oeis.org/A266363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive integer x such that n + x^2 = y^3 + z^4
 *            for some positive integers y and z; 0 if no such x exists
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------- static state for cube values ------------- */
static seq_t *cube_vals  = NULL;   /* array of perfect cubes (base^3) */
static size_t cube_count = 0;      /* how many cubes we have */
static size_t cube_cap   = 0;      /* capacity of cube_vals array */

/* ------------- helpers ----------------------------------- */

/* enlarge cube_vals so that it contains all cubes <= v   */
static void ensure_cubes_up_to(seq_t v)
{
    if (cube_count == 0) {
        /* first cube: base = 1 */
        seq_t base = 1;
        while (1) {
            seq_t base3 = base * base * base;
            if (base3 > v)
                break;
            if (cube_count == cube_cap) {
                size_t new_cap = cube_cap == 0 ? 16 : cube_cap << 1;
                seq_t *tmp = (seq_t *)realloc(cube_vals,
                                             new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* out of memory */
                    exit(EXIT_FAILURE);
                }
                cube_vals = tmp;
                cube_cap  = new_cap;
            }
            cube_vals[cube_count++] = base3;
            base++;
        }
    } else {
        /* cube_vals already has values up to (cube_count)^3
           Start from the next base                                    */
        seq_t base = cube_count + 1; /* next base to test */
        while (1) {
            seq_t base3 = base * base * base;
            if (base3 > v)
                break;
            if (cube_count == cube_cap) {
                size_t new_cap = cube_cap == 0 ? 16 : cube_cap << 1;
                seq_t *tmp = (seq_t *)realloc(cube_vals,
                                             new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* out of memory */
                    exit(EXIT_FAILURE);
                }
                cube_vals = tmp;
                cube_cap  = new_cap;
            }
            cube_vals[cube_count++] = base3;
            base++;
        }
    }
}

/* binary search for value in cube_vals; return true if found */
static bool cube_contains(seq_t val)
{
    size_t lo = 0, hi = cube_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (cube_vals[mid] < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (lo < cube_count && cube_vals[lo] == val);
}

/* ------------- main function ----------------------------------------- */
seq_t a(idx_t n)
{
    static const idx_t MAX_X = 1000000U;   /* safety bound for search   */
    seq_t cn = (seq_t)n;
    seq_t x  = 1;

    while (x <= (seq_t)MAX_X) {
        seq_t S = cn + x * x;             /* n + x^2                    */
        /* make sure we can test all cubes up to S */
        ensure_cubes_up_to(S);

        for (seq_t z = 1; ; ++z) {
            seq_t z2 = z * z;
            seq_t z4 = z2 * z2;           /* z^4                        */
            if (z4 > S)
                break;
            seq_t rem = S - z4;           /* y^3 must equal rem         */
            if (rem == 0)
                continue;                 /* we need y > 0               */

            if (cube_contains(rem))
                return x;                  /* minimal x found             */
        }
        ++x;
    }
    return (seq_t)0;                         /* no solution within bound   */
}
