/*
 * A260575  Number of ways to place 2n rooks on n X n board, 2 rooks in each
 *          row and each column, multiple rooks in a cell allowed, and exactly
 *          3 rooks below the main diagonal.
 * https://oeis.org/A260575
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A260575: Number of ways to place 2n rooks on an n×n board, 2 rooks in
   each row and each column, multiple rooks in a cell allowed, and exactly
   3 rooks below the main diagonal.

   Recurrence (valid for n ≥ 16):
        a(n) = 2600*a(n-13) - 21100*a(n-12) + 77590*a(n-11) - 171025*a(n-10)
               + 251874*a(n-9) - 261466*a(n-8) + 196626*a(n-7)
               - 108337*a(n-6) + 43682*a(n-5) - 12713*a(n-4)
               + 2592*a(n-3) - 350*a(n-2) + 28*a(n-1)
       (no additional term for n ≥ 13)

   Initial values (offset 3):
        a(3)=4, a(4)=114, a(5)=1492, a(6)=13992, a(7)=109538,
        a(8)=769632, a(9)=5050616, a(10)=31702275, a(11)=193204684,
        a(12)=1154354559, a(13)=6805263818, a(14)=39756392269,
        a(15)=230829718918
*/
#include <stdint.h>   /* for NULL, but not strictly required */

static const seq_t init[13] = {
    (seq_t)4,            /* a(3)  */
    (seq_t)114,          /* a(4)  */
    (seq_t)1492,         /* a(5)  */
    (seq_t)13992,        /* a(6)  */
    (seq_t)109538,       /* a(7)  */
    (seq_t)769632,       /* a(8)  */
    (seq_t)5050616,      /* a(9)  */
    (seq_t)31702275,     /* a(10) */
    (seq_t)193204684,    /* a(11) */
    (seq_t)1154354559,   /* a(12) */
    (seq_t)6805263818,   /* a(13) */
    (seq_t)39756392269,  /* a(14) */
    (seq_t)230829718918  /* a(15) */
};

/* Coefficients for a(n-13) .. a(n-1) (12th index corresponds to a(n-1)) */
static const int coeff[13] = {
    2600,   /* a(n-13) */
   -21100,  /* a(n-12) */
    77590,  /* a(n-11) */
   -171025, /* a(n-10) */
    251874, /* a(n-9)  */
   -261466, /* a(n-8)  */
    196626, /* a(n-7)  */
   -108337, /* a(n-6)  */
    43682,  /* a(n-5)  */
   -12713, /* a(n-4)  */
    2592,   /* a(n-3)  */
    -350,   /* a(n-2)  */
     28     /* a(n-1)  */
};

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;          /* undefined for n < 3  */

    if (n <= 15)                /* use stored initial terms */
        return init[n - 3];

    /* Sliding window of the last 13 terms a(n-13) .. a(n-1) */
    seq_t last[13];
    for (int i = 0; i < 13; ++i)
        last[i] = init[i];

    seq_t curr = 0;

    for (idx_t k = 16; k <= n; ++k) {
        /* Compute contribution from each previous term (sign handled via pos/neg sums) */
        seq_t sum_pos = 0;   /* sum of positive coefficient * term */
        seq_t sum_neg = 0;   /* sum of |negative coefficient| * term */

        for (int i = 0; i < 13; ++i) {
            int c = coeff[i];
            seq_t prod = last[i] * (seq_t)(c >= 0 ? c : -c);
            if (c >= 0)
                sum_pos += prod;
            else
                sum_neg += prod;
        }

        /* a(k) = sum_pos - sum_neg  (p(k)=0 for k ≥ 16) */
        curr = sum_pos - sum_neg;

        /* Slide window: drop oldest term, shift right, insert new term */
        for (int i = 0; i < 12; ++i)
            last[i] = last[i + 1];
        last[12] = curr;
    }

    return curr;
}
