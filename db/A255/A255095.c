/*
 * A255095  Number of (n+2)X(2+2) 0..1 arrays with no 3x3 subblock diagonal
 *          sum 1 and no antidiagonal sum 1 and no row sum 1 and no column sum
 *          1.
 * https://oeis.org/A255095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1)-a(n-2)+6*a(n-3)-12*a(n-4) *
 *            -3*a(n-5)-a(n-6)+6*a(n-7)+a(n-8)+6*a(n-9)+a(n-10) *
 *            +3*a(n-11)-3*a(n-12)-3*a(n-13)-2*a(n-14) * Initial values: *
 *            a(1)=77, a(2)=189, a(3)=539, a(4)=1475, a(5)=4075, * a(6)=11573,
 *            a(7)=32627, a(8)=91821, a(9)=259851, * a(10)=734851,
 *            a(11)=2075941, a(12)=5...
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 18, u64 n <= 39, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base values for n = 1..14 */
    static const seq_t base[15] = {
        0ULL,        /* unused, for 1‑based index */
        77ULL,       /* a(1) */
        189ULL,      /* a(2) */
        539ULL,      /* a(3) */
        1475ULL,     /* a(4) */
        4075ULL,     /* a(5) */
        11573ULL,    /* a(6) */
        32627ULL,    /* a(7) */
        91821ULL,    /* a(8) */
        259851ULL,   /* a(9) */
        734851ULL,   /* a(10) */
        2075941ULL,  /* a(11) */
        5870163ULL,  /* a(12) */
        16600435ULL, /* a(13) */
        46931203ULL  /* a(14) */
    };

    if (n <= 14)                  /* return known terms directly */
        return base[n];

    /* Keep the last 14 computed values: buf[0] = a(n-14), … , buf[13] = a(n-1) */
    seq_t buf[14];
    for (idx_t k = 0; k < 14; ++k)
        buf[k] = base[k + 1];     /* initialise with a(1)..a(14) */

    seq_t val = 0;                /* value for the current n while looping */
    for (idx_t i = 15; i <= n; ++i) {
        /* Separate positive and negative contributions to avoid underflow. */
        seq_t sum_pos = 0, sum_neg = 0;

        sum_pos += (seq_t)3 * buf[13]; /* 3*a(n-1) */
        sum_pos += (seq_t)6 * buf[11]; /* 6*a(n-3) */
        sum_pos += (seq_t)6 * buf[7];  /* 6*a(n-7) */
        sum_pos += buf[6];            /*   a(n-8) */
        sum_pos += (seq_t)6 * buf[5]; /* 6*a(n-9) */
        sum_pos += buf[4];            /*   a(n-10) */
        sum_pos += (seq_t)3 * buf[3]; /*   3*a(n-11) */

        sum_neg += buf[12];             /*   a(n-2)  */
        sum_neg += (seq_t)12 * buf[10]; /* 12*a(n-4) */
        sum_neg += (seq_t)3 * buf[9];   /* 3*a(n-5)  */
        sum_neg += buf[8];              /*   a(n-6)  */
        sum_neg += (seq_t)3 * buf[2];   /* 3*a(n-12) */
        sum_neg += (seq_t)3 * buf[1];   /* 3*a(n-13) */
        sum_neg += (seq_t)2 * buf[0];   /* 2*a(n-14) */

        val = sum_pos - sum_neg;        /* compute a(n) */

        /* Shift the buffer left and insert the new value at the end. */
        for (idx_t k = 0; k < 13; ++k)
            buf[k] = buf[k + 1];
        buf[13] = val;
    }

    return val;                  /* a(n) for n >= 15 */
}
