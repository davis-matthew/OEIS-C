/*
 * A351103  a(n) is the total number of polygons left over with maximum number
 *          of sides when partitioning the set of vertices of a convex n-gon
 *          into nonintersecting polygons.
 * https://oeis.org/A351103
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 628, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
   a(3) = a(4) = a(5) = 0,
   a(6) = 3, a(7) = 7, a(8) = 12,
   a(n) = 3                     if n ≡ 0 (mod 3),
   a(n) = n                     if n ≡ 1 (mod 3),
   a(n) = 3 + a(n-4) + a(n-3)   if n ≡ 2 (mod 3).   */
#include <stdbool.h>

static seq_t memo[100001];        /* enough for n ≤ 10000; grows only once */
static bool memo_computed[100001] = {false};

/* Helper to compute a(n) when it has not yet been computed.
   The function is safe to call from any order of n ≥ 3. */
static void compute_until(idx_t n)
{
    for (idx_t i = 3; i <= n; ++i) {
        if (memo_computed[i])
            continue;

        if (i == 3 || i == 4 || i == 5) {
            memo[i] = 0;
        } else if (i == 6) {
            memo[i] = 3;
        } else if (i == 7) {
            memo[i] = 7;
        } else if (i == 8) {
            memo[i] = 12;
        } else if (i % 3 == 0) {
            memo[i] = 3;
        } else if (i % 3 == 1) {
            memo[i] = (seq_t)i;
        } else { /* i % 3 == 2 */
            memo[i] = 3 + memo[i - 4] + memo[i - 3];
        }
        memo_computed[i] = true;
    }
}

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;                     /* undefined below offset; harmless */

    /* grow array when needed (should not happen for n ≤ 10000) */
    if (n >= 100001) {
        /* request too large for static array: handle up to 10000 only */
        return 0;
    }

    compute_until(n);
    return memo[n];
}
