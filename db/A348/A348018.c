/*
 * A348018  a(n) is the index of A064549(n) = n * Product_{p prime|n} p in the
 *          sequence of powerful numbers (A001694).
 * https://oeis.org/A348018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 816, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_cube_root, floor_sqrt, is_prime, rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of powerful numbers <= n * rad(n)
 * where rad(n) is the product of distinct prime divisors of n
 * and a powerful number is an integer whose every prime factor occurs
 * with exponent >= 2.
 *
 * Implementation finds the rank of m = n*rad(n) among powerful
 * numbers by counting how many powerful numbers are <= m.
 * A powerful number can be uniquely written as s^2 * t^3 with
 * t squarefree.  Therefore the count is:
 *   count(x) = Σ_{t squarefree, t^3 ≤ x} ⌊√(x / t^3)⌋
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../utils/rad.h"
#include "../utils/floor_cube_root.h"
#include "../utils/floor_sqrt.h"
#include "../utils/is_prime.h"

/* Helper: count powerful numbers up to x (inclusive). */
static seq_t count_powerful(seq_t x)
{
    /* R = floor(cuberoot(x)) */
    idx_t R = (idx_t)floor_cube_root(x);

    /* Allocate array of bool flags for squarefree numbers up to R */
    char *sqfree = (char *)malloc((R + 1) * sizeof(char));
    if (!sqfree) return 0;                  /* fallback if allocation fails */
    memset(sqfree, 1, R + 1);               /* initially assume all are squarefree */
    sqfree[0] = 0;                           /* 0 is not squarefree */

    /* Mark numbers that are divisible by a square */
    for (idx_t p = 2; p * p <= R; ++p) {
        if (is_prime(p)) {
            for (idx_t j = p * p; j <= R; j += p * p)
                sqfree[j] = 0;
        }
    }

    seq_t total = 0;
    for (idx_t t = 1; t <= R; ++t) {
        if (sqfree[t]) {
            seq_t t3 = (seq_t)t * t * t;           /* t^3  */
            seq_t div = x / t3;                   /* x / t^3  */
            seq_t sq = floor_sqrt(div);           /* floor sqrt(div)  */
            total += sq;                          /* add to count */
        }
    }

    free(sqfree);
    return total;
}

/* Sequence function a(n) for OEIS A348018 */
seq_t a(idx_t n)
{
    seq_t rad_n = rad(n);                 /* rad(n) : product of distinct primes of n */
    seq_t m     = (seq_t)n * rad_n;       /* m = n * rad(n) */
    return count_powerful(m);             /* rank of m among powerful numbers */
}
