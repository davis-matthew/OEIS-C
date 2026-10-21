/*
 * A250375  Number of length 2+6 0..n arrays with every seven consecutive
 *          terms having the maximum of some three terms equal to the minimum
 *          of the remaining four terms.
 * https://oeis.org/A250375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 21, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A250375
   a(n) = 8*a(n-1) - 28*a(n-2) + 56*a(n-3) - 70*a(n-4)
          + 56*a(n-5) - 28*a(n-6) + 8*a(n-7) - a(n-8)  for n > 8
   Initial values:
   a(1)=151, a(2)=3096, a(3)=24566, a(4)=119235,
   a(5)=428421, a(6)=1256106, a(7)=3179756, a(8)=7202421. */

seq_t a(idx_t n)
{
    /* handle indices smaller than the recurrence */
    if (n == 0) {
        return (seq_t)0;          /* not defined, but safe for n==0 */
    }

    /* explicit returns for the first eight terms */
    switch (n) {
        case 1: return (seq_t)151;
        case 2: return (seq_t)3096;
        case 3: return (seq_t)24566;
        case 4: return (seq_t)119235;
        case 5: return (seq_t)428421;
        case 6: return (seq_t)1256106;
        case 7: return (seq_t)3179756;
        case 8: return (seq_t)7202421;
    }

    /* For n > 8 we compute iteratively, keeping only the last 8 terms.
       The array holds: vals[0]=a(k), vals[1]=a(k-1), …, vals[7]=a(k-7). */
    seq_t vals[8] = {
        (seq_t)7202421, /* a(8) */
        (seq_t)3179756, /* a(7) */
        (seq_t)1256106, /* a(6) */
        (seq_t)428421,  /* a(5) */
        (seq_t)119235,  /* a(4) */
        (seq_t)24566,   /* a(3) */
        (seq_t)3096,    /* a(2) */
        (seq_t)151      /* a(1) */
    };

    /* Compute terms a(9) … a(n) */
    for (idx_t i = 8; i < n; ++i) {
        seq_t next = 8 * vals[0]
                     - 28 * vals[1]
                     + 56 * vals[2]
                     - 70 * vals[3]
                     + 56 * vals[4]
                     - 28 * vals[5]
                     + 8  * vals[6]
                     -     vals[7];
        /* shift the window to keep only the last 8 terms */
        for (int k = 7; k > 0; --k) vals[k] = vals[k - 1];
        vals[0] = next;
    }

    /* vals[0] now holds a(n) */
    return vals[0];
}
