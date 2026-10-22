/*
 * A354855  a(n) = floor(n*(2+sqrt(5))^n), equivalently, floor(n*phi^(3n)),
 *          where phi = (1+sqrt(5))/2 is the golden ratio.
 * https://oeis.org/A354855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*L(3n) (n odd) or n*L(3n)-1 (n even > 0)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* L(k) is the k‑th Lucas number.  The identity comes from the
   relation a(n) = floor(n*phi^(3n)), with phi = (1+sqrt5)/2.       */

#include "../utils/floor_log2_uint64.h"

/* fast doubling: returns (F(n), F(n+1)) in *fn and *fn1          */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn = 0;
        *fn1 = 1;
        return;
    }
    seq_t a = 0;   /* F(0)   */
    seq_t b = 1;   /* F(1)   */
    int top = floor_log2_uint64(n);
    for (int i = top; i >= 0; --i) {
        seq_t two_b = b << 1;      /* 2*b          */
        seq_t c = a * (two_b - a); /* a*(2*b - a)  */
        seq_t d = a * a + b * b;   /* a^2 + b^2    */
        if ((n >> i) & 1) {        /* odd bit      */
            a = d;
            b = c + d;
        } else {                   /* even bit     */
            a = c;
            b = d;
        }
    }
    *fn  = a;
    *fn1 = b;
}

/* main sequence function                                          */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    idx_t k = 3 * n;                /* index for Lucas number   */
    seq_t Fk, Fk1;
    fib_pair(k, &Fk, &Fk1);         /* F(k), F(k+1)             */

    /* L(k) = 2*F(k+1) - F(k)                                 */
    seq_t two_Fk1 = Fk1 << 1;
    seq_t Lk = two_Fk1 - Fk;

    seq_t sn = (seq_t)n;
    seq_t res = sn * Lk;            /* n*L(3n)                  */

    if ((n & 1) == 0 && n > 0) {    /* even n > 0               */
        res = res - 1;
    }

    return res;
}
