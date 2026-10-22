/*
 * A377918  a(n) = index in A377912 (or, equally, in A342042) of the first
 *          n-digit term.
 * https://oeis.org/A377918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n‑1) + b(n‑1) for n ≥ 2 a(1) = 1 The first differences
 *            b(n) satisfy b(1) = 10, b(2) = 66, b(3) = 489, b(4) = 3631, b(5)
 *            = 26951, b(6) = 200045 for n ≥ 7: b(n) = 5*b(n‑1) + 15*b(n‑2) +
 *            20*b(n‑3) + 15*b(n‑4) + 6*b(n‑5) + b(n‑6) – r(n) where r(7) = 1
 *            and r(n) = 0 for all other n. The implem...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define MAX_N 20000                      /* sufficient for all tests */

static seq_t a_arr[MAX_N + 1];           /* a[1..MAX_N] */
static seq_t b_arr[MAX_N];               /* 1..MAX_N-1 are used as b[ ] */
static bool  initialized = false;
static idx_t max_a = 0;                  /* highest a index computed */
static idx_t max_b = 0;                  /* highest b index computed */

static void init_arrays(void)
{
    /* a(1) */
    a_arr[1] = (seq_t)1;

    /* differences */
    b_arr[1] = (seq_t)10;
    b_arr[2] = (seq_t)66;
    b_arr[3] = (seq_t)489;
    b_arr[4] = (seq_t)3631;
    b_arr[5] = (seq_t)26951;
    b_arr[6] = (seq_t)200045;

    /* mark as initialized */
    max_a = 1;
    max_b = 6;
    initialized = true;
}

static void compute_b_up_to(idx_t n)
{
    /* Compute b[n] for all n > max_b up to the requested n (inclusive) */
    for (idx_t k = max_b + 1; k <= n; ++k)
    {
        /* base cases already known for k ≤ 6 */
        if (k <= 6)
            continue;

        seq_t sum = (seq_t)0;
        sum += (seq_t)5  * b_arr[k - 1];
        sum += (seq_t)15 * b_arr[k - 2];
        sum += (seq_t)20 * b_arr[k - 3];
        sum += (seq_t)15 * b_arr[k - 4];
        sum += (seq_t)6  * b_arr[k - 5];
        sum +=           b_arr[k - 6];
        if (k == 7)
            sum -= (seq_t)1;               /* r(7) = 1 */

        b_arr[k] = sum;
    }
    if (n > max_b)
        max_b = n;
}

static void compute_a_up_to(idx_t n)
{
    for (idx_t i = max_a + 1; i <= n; ++i)
    {
        a_arr[i] = a_arr[i - 1] + b_arr[i - 1];
    }
    max_a = n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;               /* out of bounds */

    if (!initialized)
        init_arrays();

    if (max_a < n)
    {
        /* Need differences up to n‑1 */
        compute_b_up_to(n - 1);
        compute_a_up_to(n);
    }
    return a_arr[n];
}
