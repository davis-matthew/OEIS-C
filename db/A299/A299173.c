/*
 * A299173  a(n) is the maximum number of squared consecutive positive
 *          integers into which the integer n can be partitioned.
 * https://oeis.org/A299173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) is the maximum length L such that n can be written as a sum
 *            of L consecutive perfect squares. The function searches all
 *            lengths L from the largest possible downwards and checks for an
 *            integer starting index a that yields the desired sum
 * Range:     u8 n <= 501, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* convert n to seq_t to avoid mixing types */
    seq_t N = (seq_t)n;
    if (N < 1) return (seq_t)0;

    /* find the largest L such that sum_{i=1}^{L} i^2 <= N */
    idx_t L = 1;
    while (1) {
        seq_t Ls   = (seq_t)L;
        seq_t sum1 = Ls * (Ls + 1) * (2 * Ls + 1) / 6;
        if (sum1 > N) break;
        L++;
    }
    idx_t Lmax = L - 1;
    if (Lmax == 0) return (seq_t)0;

    /* iterate lengths from Lmax downwards */
    for (idx_t L = Lmax; L > 0; --L) {
        seq_t Ls   = (seq_t)L;
        seq_t partC = (Ls - 1) * Ls * (2 * Ls - 1) / 6; /* constant part of sum formula */
        seq_t coef1 = Ls;                           /* coefficient of a^2   */
        seq_t coef2 = Ls * (Ls - 1);                /* coefficient of a    */

        /* try all possible starting indices a >= 1 */
        idx_t a_start = 1;
        while (1) {
            seq_t a2 = (seq_t)a_start * (seq_t)a_start;
            seq_t T  = coef1 * a2 + coef2 * (seq_t)a_start + partC;
            if (T == N) return (seq_t)L;          /* found a representation of length L */
            if (T > N) break;                     /* further a will only increase T */
            a_start++;
        }
    }
    return (seq_t)0; /* no representation found */
}
