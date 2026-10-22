/*
 * A363321  Number of fractions of the Farey sequence of order n, F_n, that
 *          coincide with those of the sequence of the #{F_n} equally
 *          distributed fractions between 0 and 1.
 * https://oeis.org/A363321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of indices i for which the i‑th fraction of the Farey
 *            sequence F_n equals the i‑th fraction of the sequence of the
 *            |F_n| equally spaced fractions between 0 and 1
 * Range:     u8 n <= 28, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   farey_size, phi_table
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --------------------------------------------------------------- */
/*  Standard headers.  The build defines seq_t and idx_t.          */
/* --------------------------------------------------------------- */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/farey_size.h"
#include "../utils/phi_table.h"

/* --------------------------------------------------------------- */
/*  Sieve for Euler's totient function up to the needed n.        */
/*  The function rebuilds the table from scratch on every call;   */
/*  for n <= 10000 this is inexpensive.                          */

/* --------------------------------------------------------------- */
/*  Compute |F_n| = 1 + sum_{k=1}^n phi(k).  Return as seq_t.     */

/* --------------------------------------------------------------- */
/*  Main function required by the task.  It counts matching indices.*/
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* not defined for n < 1 */

    /* size of Farey sequence and denominator of the equally spaced seq. */
    seq_t fn = farey_size(n);             /* |F_n| */
    seq_t M  = fn - (seq_t)1;             /* denominator of equally spaced seq. */

    /* iterate simultaneously over the Farey sequence and the equally */
    /* spaced sequence.  i is the zero‑based index.                    */
    idx_t   a_num = (idx_t)0, a_den = (idx_t)1;   /* current fraction a/b */
    idx_t   c_num = (idx_t)1, c_den = n;           /* next fraction in F_n */
    idx_t   i     = 0;                              /* current index */
    seq_t   count = 0;

    for (;;) {
        /* test equality a/b == i/M  via cross multiplication */
        unsigned long long left  = (unsigned long long)a_num * (unsigned long long)M;
        unsigned long long right = (unsigned long long)i * (unsigned long long)a_den;
        if (left == right) ++count;

        /* stop after the last fraction 1/1 */
        if (a_num == 1 && a_den == 1) break;

        /* next Farey fraction: algorithm from Knuth / standard */
        idx_t k = (n + a_den) / c_den;
        idx_t e = k * c_num - a_num;
        idx_t f = k * c_den - a_den;

        a_num = c_num;   a_den = c_den;
        c_num = e;       c_den = f;

        ++i;
    }

    return count;
}
