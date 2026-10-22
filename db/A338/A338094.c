/*
 * A338094  Number of ways to write 2*n + 1 as x^2 + y^2 + z^2 + w^2 with x +
 *          y a positive power of two, where x, y, z, w are nonnegative
 *          integers with x <= y and z <= w.
 * https://oeis.org/A338094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A338094
 *  Number of ways to write 2*n+1 as x^2 + y^2 + z^2 + w^2 where
 *  x,y,z,w are nonnegative integers, x <= y, z <= w, and x+y is a
 *  positive power of two (>=2).
 *
 *  The implementation precomputes the number of unordered representations
 *  of every integer up to 2*MAX_N+1 as a sum of two squares.  Then for
 *  each n it iterates over all powers of two s=2^k (k>=1) with s <=
 *  2*sqrt(2*n+1), enumerates all pairs (x,y) with x <= y and x+y=s,
 *  and accumulates the precomputed counts for the remaining value.
 */

#include <stdint.h>
#include <stdbool.h>

/* Adjust this if you want to handle larger n. */
#define MAX_N 10000ULL
#define MAX_M (2*MAX_N + 1)

static seq_t sum2sq_counts[MAX_M + 1];
static bool precomputed = false;

/* Integer square root of a non‑negative integer. */
static inline idx_t int_sqrt(idx_t v)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= v)
        ++r;
    return r;
}

/* Precompute unordered counts of representations as sum of two squares
 * for all integers up to MAX_M.  z,w are nonnegative and z <= w. */
static void precompute_sum2sq(void)
{
    for (idx_t z = 0; z * z <= MAX_M; ++z)
    {
        for (idx_t w = z; w * w <= MAX_M; ++w)
        {
            idx_t sum = z * z + w * w;
            if (sum > MAX_M)
                break;
            sum2sq_counts[sum] += 1;
        }
    }
    precomputed = true;
}

/* Main sequence function requested by the build. */
seq_t a(idx_t n)
{
    if (!precomputed)
        precompute_sum2sq();

    idx_t M = 2 * n + 1;                 /* target integer */
    seq_t ans = 0;

    idx_t sqrtM = int_sqrt(M);
    idx_t s = 2;                          /* first power of two >=2 */
    while (s <= 2 * sqrtM)
    {
        /* enumerate x in [0, s/2] so that x <= y = s - x */
        idx_t max_x = s >> 1;            /* floor(s/2) */
        for (idx_t x = 0; x <= max_x; ++x)
        {
            idx_t y = s - x;
            idx_t sum_xy = x * x + y * y;
            if (sum_xy > M)
                continue;
            idx_t rem = M - sum_xy;
            if (rem <= MAX_M)
                ans += sum2sq_counts[rem];
        }
        s <<= 1;                         /* next power of two */
    }

    return ans;
}
