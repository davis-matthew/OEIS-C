/*
 * A289162  Number of 4-cycles in the n X n black bishop graph.
 * https://oeis.org/A289162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n-1)*(12*n^4 + 162*n^2 + 15 - (68*n^3 + 193*n) -
 *            15*(-1)^n*(1-7*n+2*n^2)))/240
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 154, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (offset 1) */
seq_t a(idx_t n)
{
    /* For n <= 3 the value is 0 */
    if (n <= 3)
        return 0;

    seq_t sn   = (seq_t)n;       /* n as seq_t */
    seq_t n2   = sn * sn;        /* n^2 */
    seq_t n3   = n2 * sn;        /* n^3 */
    seq_t n4   = n3 * sn;        /* n^4 */

    /* Compute bracket part: 12*n^4 + 162*n^2 + 15 - (68*n^3 + 193*n) +/- 15*(1-7*n+2*n^2) */
    seq_t bracket = 12 * n4;              /* 12*n^4 */
    bracket += 162 * n2;                 /* +162*n^2 */
    bracket += 15;                       /* +15 */

    seq_t sub = 68 * n3;                 /* 68*n^3 */
    sub += 193 * sn;                     /* +193*n */

    /* (1 - 7*n + 2*n^2) = 2*n^2 - 7*n + 1  which is positive for n >= 4 */
    seq_t t1 = 2 * n2 - 7 * sn + 1;      /* 2*n^2 - 7*n + 1 */
    seq_t extra = 15 * t1;               /* 15*(1-7*n+2*n^2) */

    /* Adjust according to parity */
    if (n & 1) { /* n odd => +extra */
        bracket -= sub;
        bracket += extra;
    } else {     /* n even => -extra */
        bracket -= sub;
        bracket -= extra;
    }

    /* Evaluate ((n-1)*bracket)/240 without 64‑bit overflow.
       Decompose ((n-1)*bracket) as ((n-1)/240)*bracket + ((n-1)%240)*bracket/240.
       Further split the second part to keep intermediate products small. */
    seq_t nm1        = sn - 1;
    seq_t divn1      = nm1 / 240;
    seq_t remn1      = nm1 % 240;

    seq_t result = divn1 * bracket;                 /* (n-1)/240 * bracket */

    /* (bracket/240)*remn1 + (bracket%240)*remn1/240 */
    seq_t term2 = (bracket / 240) * remn1;
    term2 += ((bracket % 240) * remn1) / 240;

    result += term2;
    return result;
}
