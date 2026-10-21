/*
 * A273825  Table read by rows: the n-th row is the list of numbers diagonally
 *          up and to the left of n in the natural numbers read by
 *          antidiagonals.
 * https://oeis.org/A273825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 1081, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) is the n-th element of the table formed by, for each
   natural number m ≥ 1, listing the numbers on the up‑left diagonal of
   m in the antidiagonal arrangement of natural numbers.
   The table is read row‑by‑row, and this function returns the value at
   position n in that flattened reading. */
/* The implementation scans m upwards, keeping the current diagonal
   index t and the cumulative number of elements produced so far.
   When the desired index n lies in the current chain, it computes
   the offset within that chain and returns the corresponding value. */

#include <stdint.h>

/* triangular number: T(k) = k(k+1)/2, computed with 128‑bit to avoid overflow */
static inline idx_t tri(idx_t k) {
    return (idx_t)(((__int128)k * (k + 1)) >> 1);
}

seq_t a(idx_t n) {
    idx_t prev = 0;          /* cumulative elements of processed chains           */
    idx_t m    = 1;          /* current natural number (index in antidiagonal)    */
    idx_t t    = 1;          /* current diagonal index (t ≥ 1)                    */
    idx_t Tt   = tri(t);     /* T_t = t(t+1)/2, upper bound of current diagonal */

    for (;;) {
        /* Ensure t is the diagonal containing number m */
        while (m > Tt) {                 /* move to next diagonal if needed      */
            ++t;
            Tt = tri(t);
        }

        /* k = offset of m within diagonal t (0‑based) */
        idx_t k = m - (Tt - t) - 1;      /* T_t – t = T_{t‑1}                   */
        /* length of the up‑left chain from m      */
        idx_t len = 1 + (k < t - k - 1 ? k : t - k - 1);

        if (prev + len >= n) {           /* n lies in this chain                 */
            idx_t offset = n - prev - 1; /* 0‑based offset inside the chain     */
            idx_t i = k + 1;            /* current coordinates                 */
            idx_t j = t - k;

            /* coordinates of the target element      */
            idx_t i2 = i - offset;
            idx_t j2 = j - offset;

            idx_t t2 = i2 + j2 - 1;
            idx_t k2 = i2 - 1;
            idx_t val = tri(t2 - 1) + k2 + 1;

            return (seq_t)val;          /* cast to the provided seq_t type     */
        }

        /* Move to next natural number */
        prev += len;
        ++m;
    }
}
