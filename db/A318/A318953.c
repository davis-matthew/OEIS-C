/*
 * A318953  Maximum Heinz number of a strict factorization of n into factors >
 *          1.
 * https://oeis.org/A318953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the maximum Heinz number of a strict factorization of n.
   The Heinz number of a factorization (y1,…,yk) is prime(y1)*⋯*prime(yk)
   where prime(d) is the d‑th prime number.  */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

/* ---- cache for values a(n) for 0 <= n < MAX_CACHE_N ---- */
#define MAX_CACHE_N 10001
static seq_t cache_val[MAX_CACHE_N];
static unsigned char cache_used[MAX_CACHE_N];

/* ---- recursive helper ---- */
static seq_t rec(idx_t n, idx_t min_factor)
{
    if (n == 1) return (seq_t)1;

    /* use cache only for the initial call with min_factor == 2 */
    if (min_factor == 2 && n < MAX_CACHE_N && cache_used[n])
        return cache_val[n];

    seq_t best = (seq_t)0;

    /* enumerate divisors d of n with d >= min_factor */
    for (idx_t d = min_factor; d * d <= n; ++d) {
        if (n % d == 0) {
            /* factor d itself */
            seq_t sub = rec(n / d, d + 1);
            seq_t pd  = (seq_t)get_prime(d);
            seq_t prod = pd * sub;
            if (prod > best) best = prod;

            /* complementary divisor n/d if distinct */
            idx_t d2 = n / d;
            if (d2 != d && d2 >= min_factor) {
                seq_t sub2 = rec(n / d2, d2 + 1);
                seq_t pd2  = (seq_t)get_prime(d2);
                seq_t prod2 = pd2 * sub2;
                if (prod2 > best) best = prod2;
            }
        }
    }

    /* trivial factorization consisting of n itself */
    if (n >= min_factor) {
        seq_t pd = (seq_t)get_prime(n);
        if (pd > best) best = pd;
    }

    /* store in cache if appropriate */
    if (min_factor == 2 && n < MAX_CACHE_N) {
        cache_val[n] = best;
        cache_used[n] = 1;
    }

    return best;
}

/* ---- public sequence function ---- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* undefined for n < 1 */
    if (n == 1) return (seq_t)1;      /* a(1) = 1 by definition */

    return rec(n, 2);
}
