/*
 * A387497  Expansion of (1/x) * Series_Reversion( x / ((1+x)^2 *
 *          (1+x^2*(1+x)^2)) ).
 * https://oeis.org/A387497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..floor(n/2)} binomial(n+1,k) *
 *            binomial(2*n+2*k+2, n-2*k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 26, u128 n <= 50,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t kmax = n / 2;

    /*  64‑bit implementation: use 128‑bit accumulator to avoid overflow
     *  while keeping the code portable to arbitrary‑precision implementations.
     */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 sum128 = 0;

        for (idx_t k = 0; k <= kmax; ++k) {
            seq_t t1 = binomial(n + 1, k);
            seq_t t2 = binomial(2 * n + 2 * k + 2, n - 2 * k);
            sum128 += (unsigned __int128)t1 * (unsigned __int128)t2;
        }
        return (seq_t)(sum128 / (n + 1));
    } else {
        /*  Big‑integer implementation – seq_t itself is a big integer
         *  type (e.g. arbitrary‑precision or 128‑bit).  Multiplication
         *  and addition are performed directly on seq_t objects.
         */
        seq_t sum = 0;
        for (idx_t k = 0; k <= kmax; ++k) {
            seq_t t1 = binomial(n + 1, k);
            seq_t t2 = binomial(2 * n + 2 * k + 2, n - 2 * k);
            sum = sum + t1 * t2;
        }
        return sum / (n + 1);
    }
}
