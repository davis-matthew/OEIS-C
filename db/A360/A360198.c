/*
 * A360198  Number of induced cycles in the 5 X n grid graph.
 * https://oeis.org/A360198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of induced cycles in the 5 x n grid graph.
 *            Recurrence of order 15 derived from the generating function:
 *            a(n) = 6*a(n-1) - 14*a(n-2) + 17*a(n-3) - 3*a(n-4) - 17*a(n-5) +
 *            16*a(n-6) - 2*a(n-7) + 9*a(n-8) - 27*a(n-9) + 5*a(n-10) +
 *            19*a(n-11) - 3*a(n-12) - 2*a(n-13) - 5*a(n-14) + 2*a(n-1...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[16] = {
        0ULL,          /* dummy for index 0 */
        0ULL,          /* a(1) */
        4ULL,          /* a(2) */
        14ULL,         /* a(3) */
        58ULL,         /* a(4) */
        229ULL,        /* a(5) */
        749ULL,        /* a(6) */
        2180ULL,       /* a(7) */
        6188ULL,       /* a(8) */
        17912ULL,      /* a(9) */
        52620ULL,      /* a(10) */
        154383ULL,     /* a(11) */
        450195ULL,     /* a(12) */
        1309452ULL,    /* a(13) */
        3811888ULL,    /* a(14) */
        11110432ULL    /* a(15) */
    };

    if (n <= 15U)
        return init[n];

    /* buffer holding the last 15 terms: buf[0] = a(n-15), ... buf[14] = a(n-1) */
    seq_t buf[15];
    for (int i = 0; i < 15; ++i)
        buf[i] = init[i + 1];

    idx_t i = 16;
    seq_t val;
    while (i <= n)
    {
        seq_t a1  = buf[14];  /* a(n-1) */
        seq_t a2  = buf[13];  /* a(n-2) */
        seq_t a3  = buf[12];  /* a(n-3) */
        seq_t a4  = buf[11];  /* a(n-4) */
        seq_t a5  = buf[10];  /* a(n-5) */
        seq_t a6  = buf[9];   /* a(n-6) */
        seq_t a7  = buf[8];   /* a(n-7) */
        seq_t a8  = buf[7];   /* a(n-8) */
        seq_t a9  = buf[6];   /* a(n-9) */
        seq_t a10 = buf[5];   /* a(n-10) */
        seq_t a11 = buf[4];   /* a(n-11) */
        seq_t a12 = buf[3];   /* a(n-12) */
        seq_t a13 = buf[2];   /* a(n-13) */
        seq_t a14 = buf[1];   /* a(n-14) */
        seq_t a15 = buf[0];   /* a(n-15) */

        seq_t pos = 0;
        seq_t neg = 0;

        /* positive contributions */
        pos += (seq_t)6  * a1;
        pos += (seq_t)17 * a3;
        pos += (seq_t)16 * a6;
        pos += (seq_t)9  * a8;
        pos += (seq_t)5  * a10;
        pos += (seq_t)19 * a11;
        pos += (seq_t)2  * a15;

        /* negative contributions */
        neg += (seq_t)14 * a2;
        neg += (seq_t)3  * a4;
        neg += (seq_t)17 * a5;
        neg += (seq_t)2  * a7;
        neg += (seq_t)27 * a9;
        neg += (seq_t)3  * a12;
        neg += (seq_t)2  * a13;
        neg += (seq_t)5  * a14;

        val = pos - neg;

        /* shift buffer left and insert new value */
        for (int j = 0; j < 14; ++j)
            buf[j] = buf[j + 1];
        buf[14] = val;

        ++i;
    }

    return buf[14];
}
