/*
 * A293434  a(n) is the sum of the proper divisors of n that are Jacobsthal
 *          numbers (A001045).
 * https://oeis.org/A293434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d<n} d if d is a Jacobsthal number
 * Range:     u8 n <= 681, u16 n <= 9173, u32 n <= 9173, u64 n <= 9173,
 *            u128 n <= 9173, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation:   a(n) = A293432(n) – A147612(n)*n  */

#include <stdbool.h>
#include <stddef.h>

/* ------------------------------------------------------------------ */
/*  Simple cache of distinct Jacobsthal numbers (A001045) up to a     */
/*  given limit.  Only the distinct values are stored: 1, 3, 5, 11, …  */
/* ------------------------------------------------------------------ */
static idx_t jacob_arr[64];
static size_t   jcnt = 0;   /* number of values in jacob_arr              */
static idx_t   jmax = 0;   /* largest limit for which the cache is valid */

/* Integer square‑root: floor(sqrt(n)) for 0 ≤ n ≤ 2^64−1. */
static idx_t floor_sqrt_u64(idx_t n) {
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Rebuild the list of distinct Jacobsthal numbers ≤ limit. */
static void ensure_jacob(idx_t limit) {
    if (limit <= jmax)          /* cached values are still valid */
        return;

    jcnt = 0;                   /* start a new cache   */
    if (limit < 1) {
        jmax = limit;
        return;
    }

    /* J0 = 0 (ignored), J1 = 1; generate from J1 onward. */
    jacob_arr[jcnt++] = 1;
    idx_t j_prev = 0;
    idx_t j_cur  = 1;

    for (;;) {
        unsigned __int128 tmp = (unsigned __int128)j_cur + (unsigned __int128)2 * j_prev;
        if (tmp > limit)      /* further numbers will be larger   */
            break;
        idx_t j_next = (idx_t)tmp;
        /* Skip the duplicate 1 that occurs as J2.            */
        if (j_next != j_cur && jcnt < 64)
            jacob_arr[jcnt++] = j_next;
        j_prev = j_cur;
        j_cur  = j_next;
    }

    jmax = limit;               /* cache is now valid up to 'limit'  */
}

/* Linear search over the small table of Jacobsthal numbers. */
static bool is_jacob(idx_t x) {
    for (size_t i = 0; i < jcnt; ++i)
        if (jacob_arr[i] == x)
            return true;
    return false;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n <= 1)                 /* a(1) = 0, a(0) is outside the offset  */
        return 0;

    /* all proper divisors of n are < n, so it is enough to know all   */
    /* Jacobsthal numbers up to n-1.  Rebuilding a bigger cache is    */
    /* inexpensive because the list is tiny (≤ 64 entries).           */
    ensure_jacob(n);

    seq_t sum = 0;
    idx_t root = floor_sqrt_u64(n);

    for (idx_t d = 1; d <= root; ++d) {
        if (n % d == 0) {
            idx_t other = n / d;
            if (d < n && is_jacob(d))
                sum += (seq_t)d;
            if (other != d && other < n && is_jacob(other))
                sum += (seq_t)other;
        }
    }
    return sum;
}
