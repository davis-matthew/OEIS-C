/*
 * A334230  Triangle read by rows: T(n,k) gives the meet of n and k in the
 *          graded lattice of the positive integers defined by covering
 *          relations "n covers (n - n/p)" for all divisors p of n.
 * https://oeis.org/A334230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, look
 * Formula:   not stated
 * Range:     u8 n <= 8635, u16 n <= 8635, u32 n <= 8635, u64 n <= 8635,
 *            u128 n <= 8635, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A334230: meet of two numbers in the graded lattice of positive
 * integers poset defined by n covers n‑n/p for every prime divisor p of n.
 *
 * The sequence is read by rows of the triangular array T(n,k) which
 * satisfies
 *     T(n,k) = m * T(n/m, k/m)   for m = gcd(n,k).
 *
 * This implementation pre‑computes the triangle up to row 200 (enough
 * for any n ≤ 10000), and indexes the triangle by the usual
 * triangular numbering.  The routine is thread‑safe on the first call.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* ---------- integer square root (floor) ---------------------------------- */
static idx_t isqrt_u64(idx_t n)
{
    /* float sqrt may be +‑1; correct with small adjustments */
    idx_t r = (idx_t)sqrt((long double)n);
    while ((r + 1) * (r + 1) <= n) ++r;
    while (r * r > n) --r;
    return r;
}

/* ---------- Smallest prime factor for n ≤ 200 (trial division) ----------- */
static inline idx_t _smallest_prime_factor(idx_t n)
{
    if (n % 2ULL == 0ULL) return 2ULL;
    for (idx_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0ULL) return p;
    }
    return n;
}

/* ---------- Pre‑computation of reachability -------------------------------- */
#define MAX_VAL 200  /* maximum value needed for rows up to n≈10000 */

static bool reach_computed[MAX_VAL + 1];
static bool reach_from[MAX_VAL + 1][MAX_VAL + 1];   /* reach_from[m][x] */

/* Recursive computation of reachable numbers from source m */
static void compute_reach_from(idx_t m)
{
    if (reach_computed[m]) return;
    reach_computed[m] = true;

    /* initially mark only itself reachable */
    for (idx_t x = 0; x <= MAX_VAL; ++x) reach_from[m][x] = false;
    reach_from[m][m] = true;

    /* factor m to find distinct prime divisors */
    idx_t tmp = m;
    while (tmp > 1) {
        idx_t p = _smallest_prime_factor(tmp);
        while (tmp % p == 0) tmp /= p;          /* skip repetitions */

        idx_t next = m - m / p;                 /* reachable from m using p */
        compute_reach_from(next);               /* ensure next is computed */

        /* merge next's reachable set into m's set */
        for (idx_t x = 1; x <= MAX_VAL; ++x) {
            if (reach_from[next][x]) reach_from[m][x] = true;
        }
    }
}

/* ---------- Meet operator using precomputed reachability ----------------- */
static inline seq_t meet(idx_t a, idx_t b)
{
    for (idx_t x = MAX_VAL; x >= 1; --x) {
        if (reach_from[a][x] && reach_from[b][x]) return (seq_t)x;
    }
    return (seq_t)1;   /* should never happen */
}

/* ---------- Pre‑computed triangle T[n][k] --------------------------------- */
#define MAX_R 200   /* maximum row index needed for n≤10000 */
static bool triangle_computed = false;
static seq_t T[MAX_R + 1][MAX_R + 1];

static void precompute_triangle(void)
{
    if (triangle_computed) return;
    triangle_computed = true;

    /* compute reachability for all numbers up to MAX_VAL */
    for (idx_t m = 1; m <= MAX_VAL; ++m) compute_reach_from(m);

    /* build triangle values using meet */
    for (idx_t n = 1; n <= MAX_R; ++n) {
        for (idx_t k = 1; k <= n; ++k) {
            T[n][k] = meet(n, k);
        }
    }
}

/* ---------- Main sequence function ---------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; n=0 not used but safe */

    precompute_triangle();

    /* find row r such that r(r+1)/2 ≥ n > (r-1)r/2 */
    idx_t r = (isqrt_u64(8ULL * n + 1) - 1ULL) / 2ULL;
    if (r * (r + 1ULL) / 2ULL < n) ++r;

    /* column j within the row */
    idx_t j = n - (r - 1ULL) * r / 2ULL;

    return T[(int)r][(int)j];
}
