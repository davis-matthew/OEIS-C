/*
 * A308427  a(n) is the number of central factorizations needed to reach the
 *          prime factorization of n.
 * https://oeis.org/A308427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of central factorizations needed to reach the
 *            prime factorization of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Integer square‑root: floor(sqrt(n)) for 0 <= n <= 2^64-1.
   Uses only integer arithmetic, safe from overflow. */
static inline idx_t floor_sqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid == 0) {                 /* guard against division by 0 */
            lo = 1;
            continue;
        }
        if (mid > n / mid) {            /* mid*mid > n without overflow */
            hi = mid - 1;
        } else {
            if (mid + 1 > n / (mid + 1))
                return mid;
            lo = mid + 1;
        }
    }
    return hi;
}

#include "../utils/is_prime.h"

/* ------------------------------------------------------------------ */
/* Memoization storage for the sequence values */
static seq_t *memo   = NULL;
static bool *cached  = NULL;
static size_t memo_cap = 0;

/* Ensure memo tables are large enough to store a(n) for all n <= target. */
static void ensure_memo(idx_t target)
{
    if ((size_t)target + 1 <= memo_cap)
        return;
    size_t new_cap = ((size_t)target + 1) * 2;
    seq_t *new_mem = (seq_t*)realloc(memo, new_cap * sizeof(seq_t));
    bool *new_c   = (bool*)realloc(cached, new_cap * sizeof(bool));
    if (!new_mem || !new_c)
        abort();
    for (size_t i = memo_cap; i < new_cap; ++i)
        new_c[i] = false;
    memo = new_mem;
    cached = new_c;
    memo_cap = new_cap;
    /* base case: a(1) = 0 */
    if (memo_cap > 1 && !cached[1]) {
        memo[1] = (seq_t)0;
        cached[1] = true;
    }
}

/* ------------------------------------------------------------------ */
/* Main sequence function (externally linked). */
seq_t a(idx_t n)
{
    if (n <= 1)          /* a(1) = 0 */
        return 0U;
    if (is_prime(n))     /* primes produce 0 */
        return 0U;
    ensure_memo(n);
    if (cached[n])
        return memo[n];

    /* Find the largest divisor <= sqrt(n). */
    idx_t d = floor_sqrt_u64(n);
    while (d > 1 && n % d != 0)
        --d;
    idx_t e = n / d;

    /* Recursive computation of the central factorization depth. */
    seq_t val_d = a(d);
    seq_t val_e = a(e);
    seq_t res = (seq_t)(1 + (val_d > val_e ? val_d : val_e));

    memo[n] = res;
    cached[n] = true;
    return res;
}
