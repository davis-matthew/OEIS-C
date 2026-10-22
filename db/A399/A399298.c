/*
 * A399298  Number of cycles of the discrete Arnold cat map on an n X n array.
 * https://oeis.org/A399298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/T(n)) * sum_{k=0..T(n)-1} b(n,k) * * Where * T(n) =
 *            order of the matrix [[2,1],[1,1]] modulo n * b(n,0)= n^2 *
 *            b(n,k)= gcd(n, L(k))^2 if k is odd * b(n,k)= gcd(n,
 *            F(k))*gcd(n,5*F(k)) if k is even >0 * * The Fibonacci F(k) and
 *            Lucas L(k) numbers are taken modulo n. * This implementation...
 * Range:     u8 n <= 54, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   gcd, matrix_order
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/matrix_order.h"

/* ------------------------------------------------------------------ */
/*  Compute the order of the matrix M = [[2,1],[1,1]] modulo n.     */
/*  The order is the smallest positive k such that M^k ≡ I (mod n).   */
/*  n is assumed to be ≥ 2.                                         */

/* ------------------------------------------------------------------ */
/*  Main sequence function.                                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;          /* a(1) = 1 by definition */

    /* Compute the permutation order T(n). */
    idx_t T = matrix_order(n);            /* T >= 1 */

    /* Fibonacci and Lucas sequences modulo n.  Use idx_t intermediates
       to avoid overflow when seq_t is tiny. */
    idx_t f_prev = 0;                     /* F(k-1) */
    idx_t f      = 1;                     /* F(k)   */
    idx_t l_prev = 2 % n;                 /* L(k-1) */
    idx_t l      = 1 % n;                 /* L(k)   */

    /* Accumulate the sum of b(n,k).  Use idx_t to keep the total
       well within 64‑bit bounds. */
    idx_t sum = (idx_t)n * (idx_t)n;      /* b(n,0) = n^2 */

    for (idx_t k = 1; k < T; ++k) {
        if (k & 1) {                      /* odd k */
            idx_t g = (idx_t)gcd((seq_t)n, (seq_t)l);   /* gcd(n, L(k)) */
            sum += g * g;
        } else {                          /* even k > 0 */
            idx_t g1 = (idx_t)gcd((seq_t)n, (seq_t)f);   /* gcd(n, F(k)) */
            idx_t t5  = ( (idx_t)5 * f ) % n;            /* 5*F(k) mod n */
            idx_t g2 = (idx_t)gcd((seq_t)n, (seq_t)t5);
            sum += g1 * g2;
        }

        /* Advance Fibonacci: F(k+1) = F(k) + F(k-1) (mod n). */
        idx_t f_next = ( (f + f_prev) % n );
        f_prev = f;
        f     = f_next;

        /* Advance Lucas:  L(k+1) = L(k) + L(k-1) (mod n). */
        idx_t l_next = ( (l + l_prev) % n );
        l_prev = l;
        l     = l_next;
    }

    /* Final result is the average of the summed values. */
    return (seq_t)(sum / T);
}
