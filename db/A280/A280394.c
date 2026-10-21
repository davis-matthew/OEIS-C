/*
 * A280394  Number of nX4 0..2 arrays with no element unequal to a strict
 *          majority of its horizontal and antidiagonal neighbors, with the
 *          exception of exactly one element, and with new values introduced
 *          in order 0 sequentially upwards.
 * https://oeis.org/A280394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 34*a(n-1) -481*a(n-2) +3880*a(n-3) -20944*a(n-4)
 *            +82088*a(n-5) -243952*a(n-6) +563264*a(n-7) -1021840*a(n-8)
 *            +1458048*a(n-9) -1621632*a(n-10) +1371136*a(n-11)
 *            -839936*a(n-12) +335872*a(n-13) -65536*a(n-14), valid for n >
 *            15; initial terms: a(1)=2, a(2)=16, ..., a(15)=822166850058812
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t init[15] = {
    2ULL,                  /* a(1) */
    16ULL,                 /* a(2) */
    238ULL,                /* a(3) */
    3002ULL,               /* a(4) */
    35676ULL,              /* a(5) */
    410670ULL,             /* a(6) */
    4627308ULL,            /* a(7) */
    51310794ULL,           /* a(8) */
    561854184ULL,          /* a(9) */
    6090082758ULL,         /* a(10) */
    65460724868ULL,        /* a(11) */
    698687410882ULL,       /* a(12) */
    7412843182912ULL,      /* a(13) */
    78243277276350ULL,     /* a(14) */
    822166850058812ULL    /* a(15) */
};

static const long long coeff[14] = {
    34,     /* coeff for a(n-1) */
    -481,   /* coeff for a(n-2) */
    3880,   /* coeff for a(n-3) */
    -20944, /* coeff for a(n-4) */
    82088,  /* coeff for a(n-5) */
    -243952,/* coeff for a(n-6) */
    563264, /* coeff for a(n-7) */
    -1021840,/* coeff for a(n-8) */
    1458048,/* coeff for a(n-9) */
    -1621632,/* coeff for a(n-10) */
    1371136,/* coeff for a(n-11) */
    -839936,/* coeff for a(n-12) */
    335872, /* coeff for a(n-13) */
    -65536  /* coeff for a(n-14) */
};

seq_t a(idx_t n) {
    if (n <= 15) return init[n - 1];

    seq_t buf[14];
    /* buf[0] = a(n-1), buf[1] = a(n-2), ..., buf[13] = a(n-14) */
    for (idx_t i = 0; i < 14; ++i) {
        buf[i] = init[14 - i];          /* init[14] = a(15) ... init[1] = a(2) */
    }

    for (idx_t cur = 16; cur <= n; ++cur) {
#ifdef __cplusplus
        seq_t sum = 0;
        for (idx_t i = 0; i < 14; ++i) {
            if (coeff[i] >= 0)
                sum += (seq_t)coeff[i] * buf[i];
            else
                sum -= (seq_t)(-coeff[i]) * buf[i];
        }
        seq_t next = sum;
#else
        __int128 sum = 0;
        for (idx_t i = 0; i < 14; ++i) {
            sum += (__int128)coeff[i] * (__int128)buf[i];
        }
        seq_t next = (seq_t)sum;
#endif
        /* shift buffer */
        for (idx_t i = 13; i > 0; --i) buf[i] = buf[i - 1];
        buf[0] = next;
    }
    return buf[0];
}
