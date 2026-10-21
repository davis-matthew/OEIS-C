/*
 * A253137  a(n) = n*gcd(c(n), d(n)), where c(n) = composite numbers, d(n) =
 *          number of divisors of c(n).
 * https://oeis.org/A253137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * gcd(c(n), d(n)) where c(n) = nth composite number
 *            (c(1)=4, c(2)=6, …) d(n) = number of divisors of c(n)
 * Range:     u8 n <= 38, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   divisor_count_u64, gcd, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/divisor_count_u64.h"
#include "../utils/gcd.h"

/* Cache of computed composite numbers */
static uint64_t *comps = NULL;   /* array of composites */
static idx_t comp_cnt = 0;       /* number of composites stored */
static idx_t comp_cap = 0;       /* current capacity of comps array */
static idx_t next_candidate = 4; /* next integer to test for compositeness */
static bool initialized = false;

/* Ensure comps holds at least n composites (n >= 1) */
static void
ensure_composites(idx_t n)
{
    if (!initialized) {
        comp_cap = 128;
        comps = (uint64_t *)malloc(comp_cap * sizeof(uint64_t));
        if (!comps) abort();
        initialized = true;
    }

    while (comp_cnt < n) {
        /* Find the next composite number */
        while (true) {
            if (next_candidate > 1 && !is_prime(next_candidate))
                break;
            ++next_candidate;
        }

        /* Store the composite */
        if (comp_cnt == comp_cap) {
            comp_cap <<= 1;
            comps = (uint64_t *)realloc(comps, comp_cap * sizeof(uint64_t));
            if (!comps) abort();
        }
        comps[comp_cnt++] = next_candidate;
        ++next_candidate;
    }
}

/* Required function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n = 0, return 0 */

    ensure_composites(n);

    uint64_t c_val = comps[n - 1];
    seq_t c = (seq_t)c_val;
    seq_t d = divisor_count_u64(c_val);  /* number of divisors of c_val */
    seq_t g = gcd(c, d);
    seq_t result = (seq_t)n * g;
    return result;
}
