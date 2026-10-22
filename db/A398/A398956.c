/*
 * A398956  Starts of runs of 26 consecutive odd numbers that are all
 *          cubefree.
 * https://oeis.org/A398956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 855, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Sequence A398956: starts of runs of 26 consecutive odd cubefree numbers.
   Algorithm: scan odd integers, test cubefreeness by checking divisibility
   by all prime cubes ≤ current odd. Keep a sliding run length; when it
   reaches 26, record the run start (current odd – 50). */

/* Small prime sieve up to a conservatively large limit (needed only for
   the cube‑root of the odd numbers we will examine). */
static bool primes_initialized = false;
#define PRIME_LIMIT 200000   /* > cube root of the odd values needed for n≈10000 */
static uint64_t prime_cube[PRIME_LIMIT + 1];
static size_t prime_cnt = 0;

static void init_primes(void)
{
    if (primes_initialized) return;

    bool is_composite[PRIME_LIMIT + 1] = { false };
    is_composite[0] = is_composite[1] = true;
    for (size_t p = 2; p * p <= PRIME_LIMIT; ++p)
        if (!is_composite[p])
            for (size_t m = p * p; m <= PRIME_LIMIT; m += p)
                is_composite[m] = true;

    for (size_t n = 2; n <= PRIME_LIMIT; ++n)
        if (!is_composite[n]) {
            uint64_t cube = (uint64_t)n * n * n;
            prime_cube[prime_cnt++] = cube;
        }

    primes_initialized = true;
}

seq_t a(idx_t n)
{
    init_primes();

    idx_t odd = 1;          /* candidate odd number */
    idx_t run_len = 0;      /* current length of consecutive cubefree odds */
    idx_t found = 0;        /* number of runs found so far */

    while (1) {
        /* check if ‘odd’ is cubefree */
        bool cube_free = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t cube = prime_cube[i];
            if ((uint64_t)odd < cube) break;
            if ((odd % cube) == 0) { cube_free = false; break; }
        }

        if (cube_free) ++run_len;
        else           run_len = 0;

        if (run_len == 26) {
            idx_t start = odd - 50;   /* first odd in the run */
            ++found;
            if (found == n) return (seq_t)start;
            run_len = 0;              /* next run cannot overlap */
        }

        odd += 2;   /* next odd integer */
    }
}
