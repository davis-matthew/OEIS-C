/*
 * A252517  Number of (n+2)X(2+2) 0..3 arrays with every 3X3 subblock row and
 *          column sum not equal to 0 2 4 6 or 7 and every 3X3 diagonal and
 *          antidiagonal sum equal to 0 2 4 6 or 7.
 * https://oeis.org/A252517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) follows a linear recurrence of order 21 * a(n) = 17*a(n-3)
 *            - 90*a(n-6) + 188*a(n-9) - 184*a(n-12) + * 89*a(n-15) -
 *            21*a(n-18) + 2*a(n-21) for n > 24 * Initial values (offset 1):
 *            a(1)…a(24) are listed below
 * Range:     u8 none, u16 n <= 9, u32 n <= 24, u64 n <= 54, u128 n <= 114,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[25] = {
    (seq_t)0,          /* a(0) unused */
    (seq_t)791,        /* a(1) */
    (seq_t)432,        /* a(2) */
    (seq_t)798,        /* a(3) */
    (seq_t)1169,       /* a(4) */
    (seq_t)2492,       /* a(5) */
    (seq_t)5214,       /* a(6) */
    (seq_t)9069,       /* a(7) */
    (seq_t)20926,      /* a(8) */
    (seq_t)45114,      /* a(9) */
    (seq_t)80515,      /* a(10) */
    (seq_t)188382,     /* a(11) */
    (seq_t)409396,     /* a(12) */
    (seq_t)733465,     /* a(13) */
    (seq_t)1722606,    /* a(14) */
    (seq_t)3755646,    /* a(15) */
    (seq_t)6734185,    /* a(16) */
    (seq_t)15840028,   /* a(17) */
    (seq_t)34586990,   /* a(18) */
    (seq_t)62035719,   /* a(19) */
    (seq_t)146024522,  /* a(20) */
    (seq_t)319084220,  /* a(21) */
    (seq_t)572390435,  /* a(22) */
    (seq_t)1347812928, /* a(23) */
    (seq_t)2946242464  /* a(24) */
};

seq_t a(idx_t n)
{
    static seq_t *buf = NULL;
    static idx_t max_computed = 0;

    if (n == 0) return (seq_t)0;          /* undefined, return 0 */

    if (n <= max_computed)                /* already computed */
        return buf[n];

    /* allocate or extend the buffer */
    size_t new_size = (size_t)(n + 1);
    if (buf == NULL) {
        buf = (seq_t*)malloc(new_size * sizeof(seq_t));
    } else if (new_size > max_computed + 1) {
        buf = (seq_t*)realloc(buf, new_size * sizeof(seq_t));
    }

    /* initialise initial values up to min(n,24) */
    idx_t i;
    for (i = max_computed + 1; i <= (n < 24 ? n : 24); ++i)
        buf[i] = init_vals[i];

    /* compute terms beyond 24 using the recurrence */
    for (i = (idx_t)(max_computed > 24 ? max_computed : 24) + 1; i <= n; ++i) {
        seq_t pos = (seq_t)0, neg = (seq_t)0;

        /* positive coefficients */
        pos += (seq_t)17        * buf[i-3];
        pos += (seq_t)188       * buf[i-9];
        pos += (seq_t)89        * buf[i-15];
        pos += (seq_t)2         * buf[i-21];

        /* negative coefficients (added to 'neg' to subtract later) */
        neg += (seq_t)90        * buf[i-6];
        neg += (seq_t)184       * buf[i-12];
        neg += (seq_t)21        * buf[i-18];

        buf[i] = pos - neg;                 /* pos >= neg holds for all n */
    }

    max_computed = n;
    return buf[n];
}
