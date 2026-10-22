/*
 * A361432  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where T(n,k) = Sum_{j=0..floor(n/2)} k^(n-j) *
 *          binomial(n,2*j).
 * https://oeis.org/A361432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 71, u32 n <= 163, u64 n <= 396,
 *            u128 n <= 1066, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A361432
   A(n) is the n‑th element when the square array T(i,j) is read
   by antidiagonals downwards.  
   T(0,k)=1,  T(1,k)=k,  T(n,k)=2*k*T(n-1,k)-(k-1)*k*T(n-2,k).   */
/* No external utilities are needed. */

seq_t a(idx_t n)
{
    /* Find diagonal d so that d*(d+1)/2 <= n < (d+1)*(d+2)/2.
       d is the index of the diagonal (starting at 0). */
    idx_t d = 0;
    for (;; ++d) {
        idx_t next = (d + 1) * (d + 2) / 2;
        if (next > n)
            break;
    }
    /* Elements before diagonal d: d*(d+1)/2. */
    idx_t prev = d * (d + 1) / 2;
    /* Offset inside the diagonal, 0‑based. */
    idx_t offset = n - prev;
    /* Coordinates in the square array: i = offset, j = d - offset. */
    idx_t i = offset;
    idx_t j = d - offset;

    /* Compute T(i,j) using the recurrence. */
    seq_t T0 = 1;           /* T(0,j) */
    if (i == 0)
        return T0;
    seq_t T1 = (seq_t)j;    /* T(1,j) */
    if (i == 1)
        return T1;

    for (idx_t m = 2; m <= i; ++m) {
        /* a = 2*j*T1,  b = (j-1)*j*T0.  All operations are on unsigned
           types; subtraction is well‑defined because a >= b for these
           values. */
        seq_t a = 2 * j * T1;
        seq_t b = (j - 1) * j * T0;
        seq_t next = a - b;

        T0 = T1;
        T1 = next;
    }
    return T1;
}
