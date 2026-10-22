/*
 * A375512  a(n) is the number of distinct compositions of four positive
 *          integers x, u, v, y (x < u <= v < y) for which x + u + v + y = n
 *          and u*v = x*y.
 * https://oeis.org/A375512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 335, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _gcd_int_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A375512: a(n) = number of distinct quadruples
 * (x,u,v,y) of positive integers with x < u <= v < y,
 * x + u + v + y = n, and u*v = x*y.
 *
 * The implementation uses the identity
 *     n = (g + h) * (X + U)
 * where g = gcd(x,u), h = gcd(v,y),
 *     x = g*X, u = g*U, v = h*X, y = h*U
 * with X < U and gcd(X,U)=1.
 * For each divisor A=g+h of n, let B=X+U = n/A.
 *  X ranges from 1 .. floor((B-1)/2) with gcd(X,B-X)=1,
 *  and for each such X the number of valid g is
 *      t = min(A-1, floor(A*X / B)).
 * Sum t over all A and X to obtain a(n). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/_gcd_int_local.h"

/* Sequence entry; static cache for memoisation */
static seq_t *a_cache = NULL;
static idx_t a_cache_size = 0;

seq_t a(idx_t n) {
    /* Ensure cache is large enough */
    if (n >= a_cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failed; return 0 as a safe fallback */
            return 0;
        }
        a_cache = new_cache;
        /* Compute new entries sequentially */
        for (idx_t k = a_cache_size; k <= n; ++k) {
            if (k == 0) {
                a_cache[k] = 0;
                continue;
            }
            seq_t total = 0;
            /* Enumerate divisors A of k */
            for (idx_t d = 1; d * d <= k; ++d) {
                if (k % d != 0)
                    continue;
                /* First divisor pair (A=d, B=k/d) */
                idx_t A = d;
                idx_t B = k / d;

                /* Compute contributions for this A,B */
                idx_t maxX = (B - 1) / 2;
                for (idx_t X = 1; X <= maxX; ++X) {
                    idx_t U = B - X;
                    if (_gcd_int_local((int)X, (int)U) != 1)
                        continue;
                    /* t = floor(A*X / B) */
                    seq_t t = ((seq_t)A * X) / B;
                    if (t == 0)
                        continue;
                    if (t > A - 1)
                        t = A - 1;
                    total += t;
                }

                /* Second divisor pair (A=k/d, B=d) if distinct */
                if (d * d == k)
                    continue; /* skip duplicate when k is a perfect square */
                idx_t A2 = k / d;
                idx_t B2 = d;
                idx_t maxX2 = (B2 - 1) / 2;
                for (idx_t X = 1; X <= maxX2; ++X) {
                    idx_t U = B2 - X;
                    if (_gcd_int_local((int)X, (int)U) != 1)
                        continue;
                    seq_t t = ((seq_t)A2 * X) / B2;
                    if (t == 0)
                        continue;
                    if (t > A2 - 1)
                        t = A2 - 1;
                    total += t;
                }
            }
            a_cache[k] = total;
        }
        a_cache_size = new_size;
    }
    return a_cache[n];
}
