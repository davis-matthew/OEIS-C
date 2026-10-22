/*
 * A333575  Number of Hamiltonian paths in the n X 3 grid graph which start at
 *          any of the n vertices on left side of the graph and terminate at
 *          any of the n vertices on the right side.
 * https://oeis.org/A333575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2*a(n-1) + 4*a(n-2) - 8*a(n-3) - 4*a(n-4) + 8*a(n-5)
 * Initial values: a(1)=1, a(2)=2, a(3)=8, a(4)=14, a(5)=38,
 *                  a(6)=74, a(7)=170, a(8)=338
 * (Recurrence applies for n > 8)
 */

#include <stdint.h>
#include <stdlib.h>

/* Maximum n expected in the test harness (≈10000).  Adjust if needed. */
#define A_MAX_N 10000

/* Cache for computed values.  Index 0 is unused. */
static seq_t a_cache[A_MAX_N + 1] = {0};

/* Highest n that has been fully computed (inclusive). */
static idx_t a_cache_max = 0;

/* The known first eight terms. */
static const seq_t a_base[8] = {
    1, 2, 8, 14, 38, 74, 170, 338
};

/* Ensure the cache contains the first eight terms. */
static void a_init_base(void)
{
    if (a_cache_max >= 8)
        return;
    for (idx_t i = 1; i <= 8; ++i)
        a_cache[i] = a_base[i - 1];
    if (a_cache_max < 8)
        a_cache_max = 8;
}

/* Compute a(n) for any n >= 1, memoizing intermediate results. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;     /* a(0) is unused but defined for safety */

    if (n <= 8)
        return a_base[n - 1];

    /* Guard against overly large requested indices. */
    if (n > A_MAX_N) {
        /* Fallback: compute iteratively without storing beyond the requested n. */
        /* This path is rarely used and intentionally simple. */
        idx_t i;
        seq_t t1, t2, t3, t4, t5;
        seq_t prev[5] = {a_base[7], a_base[6], a_base[5], a_base[4], a_base[3]}; /* a(8)..a(4) */
        seq_t result = 0;
        for (i = 9; i <= n; ++i) {
            t1 = prev[0] * 2u;                      /* 2*a(i-1) */
            t2 = prev[1] * 4u;                      /* 4*a(i-2) */
            t3 = prev[2] * 8u;                      /* 8*a(i-3) */
            t4 = prev[3] * 4u;                      /* 4*a(i-4) */
            t5 = prev[4] * 8u;                      /* 8*a(i-5) */
            result = t1 + t2 + t5 - t3 - t4;
            /* shift history */
            prev[4] = prev[3];
            prev[3] = prev[2];
            prev[2] = prev[1];
            prev[1] = prev[0];
            prev[0] = result;
        }
        return result;
    }

    /* Normal case: n <= A_MAX_N.  Use static cache. */
    a_init_base();

    /* Compute any missing values up to n. */
    if (a_cache_max < n) {
        for (idx_t i = a_cache_max + 1; i <= n; ++i) {
            seq_t t1 = a_cache[i - 1] * 2u;          /* 2*a(i-1) */
            seq_t t2 = a_cache[i - 2] * 4u;          /* 4*a(i-2) */
            seq_t t3 = a_cache[i - 3] * 8u;          /* 8*a(i-3) */
            seq_t t4 = a_cache[i - 4] * 4u;          /* 4*a(i-4) */
            seq_t t5 = a_cache[i - 5] * 8u;          /* 8*a(i-5) */
            a_cache[i] = t1 + t2 + t5 - t3 - t4;
        }
        a_cache_max = n;
    }

    return a_cache[n];
}
