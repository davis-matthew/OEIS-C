/*
 * A302724  Number of nX3 0..1 arrays with every element equal to 1, 2, 4 or 6
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)+5*a(n-2)+27*a(n-3)-64*a(n-4)-67*a(n-5)+25*a(n-6)
 *            -22*a(n-7)-66*a(n-8)-240*a(n-9)+232*a(n-10)+168*a(n-11)-16*a(n-12)
 *            +64*a(n-13)+64*a(n-14)+256*a(n-15)-256*a(n-16) Initial values:
 *            a(1)=1, a(2)=7, a(3)=22, a(4)=83, a(5)=301, a(6)=1079,
 *            a(7)=4064, a(8)=15183, a(9)=55012, a(10)=207349...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Base terms a(1)..a(16) */
    static const seq_t init[16] = {
        (seq_t)1,          /* a(1) */
        (seq_t)7,          /* a(2) */
        (seq_t)22,         /* a(3) */
        (seq_t)83,         /* a(4) */
        (seq_t)301,        /* a(5) */
        (seq_t)1079,       /* a(6) */
        (seq_t)4064,       /* a(7) */
        (seq_t)15183,      /* a(8) */
        (seq_t)55012,      /* a(9) */
        (seq_t)207349,     /* a(10) */
        (seq_t)770421,     /* a(11) */
        (seq_t)2830250,    /* a(12) */
        (seq_t)10617371,   /* a(13) */
        (seq_t)39400644,   /* a(14) */
        (seq_t)145704104,  /* a(15) */
        (seq_t)544623685   /* a(16) */
    };

    /* Coefficients of the recurrence (positives and negatives)
       coeff[k] multiplies a(n-(k+1)) where k=0..15  */
    static const int coeff[16] = {
        2,    /* a(n-1)  */
        5,    /* a(n-2)  */
        27,   /* a(n-3)  */
        -64,  /* a(n-4)  */
        -67,  /* a(n-5)  */
        25,   /* a(n-6)  */
        -22,  /* a(n-7)  */
        -66,  /* a(n-8)  */
        -240, /* a(n-9)  */
        232,  /* a(n-10) */
        168,  /* a(n-11) */
        -16,  /* a(n-12) */
        64,   /* a(n-13) */
        64,   /* a(n-14) */
        256,  /* a(n-15) */
        -256  /* a(n-16) */
    };

    /* Return the value directly for the first 16 terms */
    if (n <= 16) {
        return init[n-1];
    }

    /* Circular buffer holding the last 16 computed terms.
       vals[0] = a(current), vals[1] = a(current-1), …, vals[15] = a(current-15) */
    seq_t vals[16];
    for (unsigned j = 0; j < 16; ++j) {
        vals[j] = init[15-j];
    }

    /* Compute subsequent terms up to a(n) */
    for (idx_t i = 17; i <= n; ++i) {
        /* Separate positive and negative contributions to avoid underflow */
        seq_t pos = 0;
        seq_t neg = 0;
        for (unsigned k = 0; k < 16; ++k) {
            if (coeff[k] >= 0) {
                pos += (seq_t)coeff[k] * vals[k];
            } else {
                neg += (seq_t)(-coeff[k]) * vals[k];
            }
        }
        seq_t next = pos - neg;   /* pos > neg by the definition of the sequence */

        /* Shift buffer: discard the oldest term (vals[15]) */
        for (unsigned j = 15; j > 0; --j) {
            vals[j] = vals[j-1];
        }
        vals[0] = next;           /* new most recent term */
    }

    return vals[0];
}
