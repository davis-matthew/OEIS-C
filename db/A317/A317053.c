/*
 * A317053  Complete list of solutions to y^2 + y = x^3 - 525x + 10156 with y
 *          > 0; sequence gives positive y values.
 * https://oeis.org/A317053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A317053:  Positive y‑values of integer points
   on the elliptic curve  y^2 + y = x^3 – 525x + 10156.
   The curve has finitely many integer points; the function returns
   the nth smallest positive y (1‑based).  If n exceeds the number
   of solutions, 0 is returned.  The list is computed by an exhaustive
   search for integer x in a reasonable range; all distinct y are
   kept, sorted, and memoised.  Formula: a(n) = nth smallest y such that
   y^2 + y = x^3 – 525x + 10156 for some integer x. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define XMAX 1000000LL   /* search range for x */

static seq_t *solutions = NULL;
static size_t sol_cnt = 0;
static bool initialized = false;

/* Simple integer square root for 64‑bit numbers. */
static inline uint64_t isqrt64(uint64_t v)
{
    uint64_t low = 0, high = 1ULL << 32;          /* sqrt(2^64-1) < 2^32 */
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        if (mid > 0 && mid > v / mid)   /* mid*mid > v, avoid overflow */
            high = mid - 1;
        else
            low = mid;
    }
    return low;
}

/* Comparator for qsort on seq_t values. */
static int cmp_seq_t(const void *a, const void *b)
{
    seq_t x = *((const seq_t *)a);
    seq_t y = *((const seq_t *)b);
    return (x < y) ? -1 : (x > y) ? 1 : 0;
}

/* Initialise the static solution list. */
static void init_solutions(void)
{
    if (initialized) return;

    /* Temporary array big enough for all solutions. */
    static seq_t tmp[2000];
    size_t tmp_cnt = 0;

    for (int64_t x = -XMAX; x <= XMAX; ++x) {
        int64_t rhs = (int64_t)x * x * x - 525LL * x + 10156LL;
        int64_t d   = 1 + 4 * rhs;
        if (d < 0) continue;                     /* discriminant negative */

        uint64_t ui = (uint64_t)d;
        uint64_t sqrt_d = isqrt64(ui);
        if ((uint64_t)sqrt_d * sqrt_d != ui)     /* not a perfect square */
            continue;

        /* sqrt_d must be odd so that (-1+sqrt_d)/2 is integer */
        if ((sqrt_d & 1ULL) == 0) continue;

        /* Compute y = (sqrt_d - 1) / 2, cast to seq_t. */
        seq_t y = (seq_t)((sqrt_d - 1) >> 1);
        if (y == 0) continue;                    /* y must be positive */

        /* Skip duplicate y values. */
        bool dup = false;
        for (size_t i = 0; i < tmp_cnt; ++i)
            if (tmp[i] == y) { dup = true; break; }
        if (!dup) tmp[tmp_cnt++] = y;
    }

    /* Sort the distinct y values. */
    qsort(tmp, tmp_cnt, sizeof(seq_t), cmp_seq_t);

    /* Allocate persistent array. */
    solutions = (seq_t *)malloc(tmp_cnt * sizeof(seq_t));
    for (size_t i = 0; i < tmp_cnt; ++i) solutions[i] = tmp[i];
    sol_cnt = tmp_cnt;
    initialized = true;
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n = 0 */
    init_solutions();
    if (n - 1 >= sol_cnt) return 0;  /* no more terms in the sequence */
    return solutions[n - 1];
}
