/*
 * A394484  Chebyshev distance between prime(n) and prime(n+1) on Ulam spiral.
 * https://oeis.org/A394484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Chebyshev distance between prime(n) and prime(n+1) on the Ulam spiral. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

/* static state ------------------------------------------------------------- */
static uint64_t *primes = NULL;      /* array of primes */
static size_t   prime_cnt = 0;       /* number of primes in the array */

/* integer square root ------------------------------------------------------ */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid <= n / mid)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* ensure enough primes are available (0‑based index).  */
static void ensure_primes(idx_t need)
{
    if (prime_cnt > need)
        return;

    /* Rough upper bound for the (need)-th prime.  */
    idx_t limit;
    if (need < 6)
        limit = 100;
    else
        limit = need * 12 + 1000;   /* safety margin */

    uint64_t *new_primes = NULL;
    size_t    new_cnt    = 0;

    build_primes_upto(limit, &new_primes, &new_cnt);

    if (primes)
        free(primes);
    primes   = new_primes;
    prime_cnt = new_cnt;
}

/* coordinates on the Ulam spiral ------------------------------- */
static void coord(idx_t m, long long *x, long long *y)
{
    if (m == 1) {
        *x = 0;
        *y = 0;
        return;
    }

    idx_t sq = isqrt_u64(m);
    if (sq * sq < m)
        ++sq;

    idx_t k  = sq / 2;                           /* layer number */
    idx_t mm = 2 * k + 1;                        /* side length */
    mm = mm * mm;                                /* (2k+1)^2 */
    idx_t offset = mm - m;                       /* offset in the layer */
    idx_t side = 2 * k;                          /* side length of the square */

    if (offset < side) {                         /* bottom edge      */
        *x = (long long)k - (long long)offset;
        *y = -(long long)k;
    } else if (offset < 2 * side) {              /* left edge        */
        *x = -(long long)k;
        *y = -(long long)k + (long long)(offset - side);
    } else if (offset < 3 * side) {              /* top edge         */
        *x = -(long long)k + (long long)(offset - 2 * side);
        *y = (long long)k;
    } else {                                     /* right edge       */
        *x = (long long)k;
        *y = (long long)k - (long long)(offset - 3 * side);
    }
}

/* main sequence function --------------------------------------------------- */
seq_t a(idx_t n)
{
    /* primes are 0‑based: primes[0] = 2, primes[1] = 3, ... */
    ensure_primes(n + 1);                 /* need p_n and p_(n+1) */

    uint64_t p1 = primes[n - 1];
    uint64_t p2 = primes[n];

    long long x1, y1, x2, y2;
    coord(p1, &x1, &y1);
    coord(p2, &x2, &y2);

    long long dx = x2 - x1;
    long long dy = y2 - y1;
    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;

    long long dist = (dx > dy) ? dx : dy;
    return (seq_t)dist;
}
