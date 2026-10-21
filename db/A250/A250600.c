/*
 * A250600  Number of (n+2)X(7+2) 0..2 arrays with nondecreasing sum of every
 *          three consecutive values in every row and column.
 * https://oeis.org/A250600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 6, u128 n <= 19,
 *            big any n
 * Helpers:   N_of_R
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/N_of_R.h"
/* 
 * Formula: a(n) = (∏_{i=0}^{2} N( floor((n+1-i)/3)+1 ))³,
 *   where N(R) = ((R+3)² * (R+2)² * (R+1) * (R+4)) / 144.
 * This counts arrays of size (n+2)×9 with entries 0‒2 such that the sum of any
 * three consecutive values in every row and column is nondecreasing.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* unused, gives safe behaviour */

    idx_t R0 = (n + 1) / 3 + 1;      /* rows with indices 0,3,6,… */
    idx_t R1 =    n   / 3 + 1;      /* indices 1,4,7,…   */
    idx_t R2 = (n - 1) / 3 + 1;      /* indices 2,5,8,…   */

    seq_t N0 = N_of_R(R0);
    seq_t N1 = N_of_R(R1);
    seq_t N2 = N_of_R(R2);

    seq_t prod = N0 * N1 * N2;
    return prod * prod * prod;       /* (N0·N1·N2)³ */
}
