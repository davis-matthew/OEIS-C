/*
 * A280600  Number of n X 4 0..1 arrays with no element equal to more than two
 *          of its horizontal, vertical and antidiagonal neighbors and with
 *          new values introduced in order 0 sequentially upwards.
 * https://oeis.org/A280600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence: a(n) = 4*a(n-1) - 4*a(n-2) + 2*a(n-5) -
 *            3*a(n-7) + 2*a(n-8) + a(n-9) - a(n-10) for n > 13 Initial
 *            values: a(1)=8, a(2)=50, a(3)=116, a(4)=302, a(5)=726,
 *            a(6)=1709, a(7)=3996, a(8)=9346, a(9)=21891, a(10)=51367,
 *            a(11)=120680, a(12)=283746, a(13)=667466
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 23, u64 n <= 49, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A280600(a(n)) for n >= 1.
   No dynamic allocation and works for all valid seq_t types. */
seq_t a(idx_t n)
{
    if (n < 1)
        abort();                     /* sequence defined only for n ≥ 1 */

    /* initial values (index 0 unused) */
    static const seq_t init[14] = {
        0,          /* dummy 0 */
        8,          /* a(1) */
        50,         /* a(2) */
        116,        /* a(3) */
        302,        /* a(4) */
        726,        /* a(5) */
        1709,       /* a(6) */
        3996,       /* a(7) */
        9346,       /* a(8) */
        21891,      /* a(9) */
        51367,      /* a(10) */
        120680,     /* a(11) */
        283746,     /* a(12) */
        667466      /* a(13) */
    };

    /* if within the known initial part, return directly */
    if (n <= 13)
        return init[n];

    /* ring buffer holding the last 11 computed terms:
       last[0] = a(i-1), last[1] = a(i-2), ..., last[10] = a(i-11)  */
    seq_t last[11];

    /* load the buffer with the 11 terms preceding n = 14 */
    /* For i = 14: need a[13] down to a[4] */
    last[0] = init[13];
    last[1] = init[12];
    last[2] = init[11];
    last[3] = init[10];
    last[4] = init[9];
    last[5] = init[8];
    last[6] = init[7];
    last[7] = init[6];
    last[8] = init[5];
    last[9] = init[4];
    last[10] = init[3];

    seq_t val;
    for (idx_t i = 14; i <= n; ++i) {
        /* recurrence using the current contents of last */
        val = 4 * last[0] - 4 * last[1] + 2 * last[4]
              - 3 * last[6] + 2 * last[7] + last[8] - last[9];

        /* shift the buffer for the next iteration */
        for (int k = 10; k >= 1; --k)
            last[k] = last[k - 1];
        last[0] = val;
    }
    return val;   /* val holds a(n) after the loop */
}
