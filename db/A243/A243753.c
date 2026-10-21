/*
 * A243753  Number A(n,k) of Dyck paths of semilength n avoiding the
 *          consecutive step pattern given by the binary expansion of k, where
 *          1=U=(1,1) and 0=D=(1,-1); square array A(n,k), n>=0, k>=0, read by
 *          antidiagonals.
 * https://oeis.org/A243753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 113, u16 n <= 224, u32 n <= 517, u64 n <= 1315,
 *            u128 n <= 3816, big any n
 * Helpers:   count_dyck_without_pattern
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A243753
 *  a(n) is the number of Dyck paths of semilength i that avoid the
 *  consecutive step pattern given by the binary expansion of j,
 *  where (i,j) are the antidiagonal coordinates of n.
 *  The function computes a(n) by constructing an automaton for the
 *  forbidden pattern and dynamic‑programming over the path steps.
 *
 *  The code is written in a form that compiles under both C11
 *  and C++17.  No library functions other than the minimal
 *  standard headers are used.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/count_dyck_without_pattern.h"

/* compute antidiagonal coordinates:  a(n) = A(i,j) for the array
 *  displayed in the OEIS entry.  i is the offset on the diagonal,
 *  j is the remaining index on the same antidiagonal.                */
static void antidiagonal_coords(idx_t n, idx_t *i, idx_t *j)
{
    idx_t t = 0;
    while (true) {
        /* (t+1)*(t+2)/2  is strictly increasing in t */
        __int128 v = (__int128)(t + 1) * (t + 2) / 2;
        if (v > n) break;
        t++;
    }
    idx_t prev_sum = t * (t + 1) / 2;   /* sum of 1..t */
    idx_t offset   = n - prev_sum;
    *i = offset;
    *j = t - offset;
}

/* public entry point for the OEIS sequence */
seq_t a(idx_t n)
{
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);
    return count_dyck_without_pattern(i, j);
}
