/*
 * A323971  Number of 7 X n integer matrices (m_{i,j}) such that m_{1,1}=0,
 *          m_{7,n}=2, and all rows, columns, and falling diagonals are
 *          (weakly) monotonic without jumps of 2.
 * https://oeis.org/A323971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 16, u64 n <= 97, u128 n <= 2407,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A323971: a(n) = Number of 7×n integer matrices with the stated
   constraints.
   The sequence satisfies a linear recurrence of order 15:
     for n >= 16
       a(n) = Σ_{k=1}^{15} (-1)^{k+1} binom(15,k) * a(n-k)
   with initial values a(0)…a(15) listed below. */

seq_t a(idx_t n)
{
    /* Initial values a(0) … a(15) */
    static const seq_t init[16] = {
        1ULL,          /* a(0) */
        15ULL,         /* a(1) */
        155ULL,        /* a(2) */
        1332ULL,       /* a(3) */
        8342ULL,       /* a(4) */
        41586ULL,      /* a(5) */
        174844ULL,     /* a(6) */
        642815ULL,     /* a(7) */
        2117690ULL,    /* a(8) */
        6362806ULL,    /* a(9) */
        17671203ULL,   /* a(10) */
        45844681ULL,   /* a(11) */
        112047610ULL,  /* a(12) */
        259796057ULL,  /* a(13) */
        574776968ULL,  /* a(14) */
        1219349012ULL  /* a(15) */
    };

    /* Binomial coefficients C(15,k), k = 1 … 15 */
    static const seq_t coeff[16] = {
        0ULL,          /* unused (k = 0) */
        15ULL,         /* C(15,1) */
        105ULL,        /* C(15,2) */
        455ULL,        /* C(15,3) */
        1365ULL,       /* C(15,4) */
        3003ULL,       /* C(15,5) */
        5005ULL,       /* C(15,6) */
        6435ULL,       /* C(15,7) */
        6435ULL,       /* C(15,8) */
        5005ULL,       /* C(15,9) */
        3003ULL,       /* C(15,10) */
        1365ULL,       /* C(15,11) */
        455ULL,        /* C(15,12) */
        105ULL,        /* C(15,13) */
        15ULL,         /* C(15,14) */
        1ULL           /* C(15,15) */
    };

    if (n < 16)
        return init[(int)n];

    /* buf holds the most recent 15 terms a(n-15)…a(n-1) */
    seq_t buf[15];
    /* initialize buffer with a(1) … a(15) (for n = 16) */
    for (int i = 0; i < 15; ++i)
        buf[i] = init[1 + i];

    for (idx_t i = 16; i <= n; ++i) {
        seq_t pos = 0, neg = 0;
        for (int k = 1; k <= 15; ++k) {
            /* a(i-k) is stored at buf[(i-1-k+15)%15] */
            seq_t val   = buf[(i - 1 - k + 15) % 15];
            seq_t prod  = coeff[k] * val;
            if (k & 1)      /* odd k → positive coefficient */
                pos += prod;
            else            /* even k → negative coefficient */
                neg += prod;
        }
        buf[(i - 1) % 15] = pos - neg;   /* a(i) */
    }

    return buf[(n - 1) % 15];
}
