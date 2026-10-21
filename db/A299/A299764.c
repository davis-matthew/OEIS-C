/*
 * A299764  Number of special products of factorizations of n into factors >
 *          1.
 * https://oeis.org/A299764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   compute_special_from_factors, div_index, divisors_u64, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute A299764: Number of special products of factorizations of n into factors > 1
 *  The function enumerates all multiplicative factorizations of n (up to order)
 *  and, for each factorization, counts the subset products that occur exactly once.
 *  It uses a divisor‑based DP to determine the multiplicities of all products.
 *  The implementation is O(number of factorizations × (number of divisors)²), which
 *  is easily fast enough for n ≤ 10000.  The code is C11/C++17 compatible
 *  and respects the required types seq_t and idx_t.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/int_sqrt.h"   /* for int_sqrt(idx_t) */
#include "../utils/compute_special_from_factors.h"
#include "../utils/div_index.h"
#include "../utils/divisors_u64.h"

/* Recursive enumeration of factorizations of n.  The result is accumulated
 * into *acc.  "min_factor" enforces non‑decreasing order to avoid duplicates. */
static void rec_fact(idx_t n, idx_t min_factor, uint64_t *cur, size_t len,
                     const uint64_t *divs, size_t divcnt, seq_t *acc)
{
    /* Add the trivial factorization containing n itself */
    cur[len] = (uint64_t)n;
    *acc += compute_special_from_factors(cur, len + 1, divs, divcnt);

    /* Enumerate further factorizations by trying a prefix factor f */
    idx_t limit = int_sqrt(n);
    for (idx_t f = min_factor; f <= limit; ++f) {
        if (n % f == 0) {
            cur[len] = (uint64_t)f;
            rec_fact(n / f, f, cur, len + 1, divs, divcnt, acc);
        }
    }
}

/* Public entry point for the sequence */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n < 1)  return (seq_t)0;

    /* Compute all divisors of n */
    uint64_t divs[256];
    size_t   divcnt;
    divisors_u64(n, divs, &divcnt);

    /* Buffer to hold the current factorization (max length < 64 for n ≤ 10000) */
    uint64_t cur[64];

    seq_t total = 0;
    rec_fact(n, 2, cur, 0, divs, divcnt, &total);
    return total;
}
