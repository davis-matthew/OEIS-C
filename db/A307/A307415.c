/*
 * A307415  Total number of parts in all symmetric m-color cyclic compositions
 *          of n (that is, the total number of parts in all achiral cyclic
 *          compositions of n where a part with size m can be colored with one
 *          of m colors).
 * https://oeis.org/A307415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/5)*[ n*(11*F(n)+7*F(n-1)) * +
 *            (-1)^n*n*(-4*F(n)+7*F(n-1)) * - 2*(5*F(n+1)+3*F(n)) * -
 *            (-1)^n*2*(3*F(n)-5*F(n-1)) ] * where F(k) = A000045(k) is the
 *            k‑th Fibonacci number. * * The function uses fast‑doubling to
 *            compute F(n) and F(n+1) in O(log n) * and then evaluates the
 *            numerator with o...
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 34, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/*  Fast doubling Fibonacci: sets *a = F(n), *b = F(n+1)   */
static void fib_pair_rec(idx_t n, seq_t *a, seq_t *b)
{
    if (n == 0) {
        *a = 0;
        *b = 1;
        return;
    }
    seq_t a1, b1;
    fib_pair_rec(n >> 1, &a1, &b1);

    seq_t two_b1 = b1 + b1;                     /* 2*F(k+1)            */
    seq_t c = a1 * (two_b1 - a1);              /* F(2k)                */
    seq_t d = a1 * a1 + b1 * b1;               /* F(2k+1)              */

    if ((n & 1) == 0) {
        *a = c;
        *b = d;
    } else {
        *a = d;
        *b = c + d;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* undefined for n=0; return 0 as safe value */

    seq_t Fn, Fn1;                             /* F(n), F(n+1)          */
    fib_pair_rec(n, &Fn, &Fn1);

    seq_t Fm1 = Fn1 - Fn;                      /* F(n-1)                */

    /*  Term 1: n*(11*Fn + 7*Fm1)                  */
    seq_t term1 = (seq_t)n * ((seq_t)11 * Fn + (seq_t)7 * Fm1);

    /*  Term 3 (always negative in the sum): -2*(5*Fn1+3*Fn)  */
    seq_t term3_neg = (seq_t)2 * ((seq_t)5 * Fn1 + (seq_t)3 * Fn);

    seq_t pos = term1;  /* positive contributions          */
    seq_t neg = term3_neg; /* negative contributions          */

    bool is_even = (n & 1) == 0;

    if (is_even) { /* ------- even n ------- */
        /* Term 2: + n*(7*Fm1 - 4*Fn)   (positive) */
        seq_t diff2 = (seq_t)7 * Fm1 - (seq_t)4 * Fn;
        pos += (seq_t)n * diff2;

        /* Term 4: + 2*(5*Fm1 - 3*Fn)  (positive) */
        seq_t diff4 = (seq_t)5 * Fm1 - (seq_t)3 * Fn;
        pos += (seq_t)2 * diff4;
    } else { /* ------- odd n --------- */
        /* Term 2 may be positive or negative */
        seq_t diff2_4 = (seq_t)4 * Fn;
        seq_t diff2_7 = (seq_t)7 * Fm1;
        if (diff2_4 >= diff2_7) {
            pos += (seq_t)n * (diff2_4 - diff2_7);
        } else {
            neg += (seq_t)n * (diff2_7 - diff2_4);
        }

        /* Term 4 may be positive or negative */
        seq_t diff4_3 = (seq_t)3 * Fn;
        seq_t diff4_5 = (seq_t)5 * Fm1;
        if (diff4_3 >= diff4_5) {
            pos += (seq_t)2 * (diff4_3 - diff4_5);
        } else {
            neg += (seq_t)2 * (diff4_5 - diff4_3);
        }
    }

    /*  The numerator is (pos - neg), which is always non‑negative */
    seq_t numerator = pos - neg;

    /*  Division by 5 is exact by the formula */
    return numerator / 5;
}
