/*
 * A250339  Number of length 3+5 0..n arrays with every six consecutive terms
 *          having the maximum of some three terms equal to the minimum of the
 *          remaining three terms.
 * https://oeis.org/A250339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 25, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A250339
   a(n) = 8*a(n-1) - 28*a(n-2) + 56*a(n-3) - 70*a(n-4)
          + 56*a(n-5) - 28*a(n-6) + 8*a(n-7) - a(n-8)   (for n>8)
   Initial values:
   a(1)=108, a(2)=1989, a(3)=14040, a(4)=62041,
   a(5)=206724, a(6)=569693, a(7)=1369136, a(8)=2966769
   All indices start at 1 (offset 1). */

seq_t a(idx_t n)
{
    /* Handle the first eight values directly. */
    static const seq_t init[8] = {
        (seq_t)108,      /* a(1) */
        (seq_t)1989,     /* a(2) */
        (seq_t)14040,    /* a(3) */
        (seq_t)62041,    /* a(4) */
        (seq_t)206724,   /* a(5) */
        (seq_t)569693,   /* a(6) */
        (seq_t)1369136,  /* a(7) */
        (seq_t)2966769   /* a(8) */
    };

    if (n <= 8) {
        return init[n - 1];
    }

    /* Sliding window of the last eight terms. */
    seq_t v[8];
    for (idx_t i = 0; i < 8; ++i) {
        v[i] = init[i];
    }

    /* Iterate from 9 up to n, updating the window each step. */
    for (idx_t i = 9; i <= n; ++i) {
        seq_t next = (seq_t)8  * v[7]
                   - (seq_t)28 * v[6]
                   + (seq_t)56 * v[5]
                   - (seq_t)70 * v[4]
                   + (seq_t)56 * v[3]
                   - (seq_t)28 * v[2]
                   + (seq_t)8  * v[1]
                   - v[0];

        /* Shift left by one position. */
        for (idx_t j = 0; j < 7; ++j) {
            v[j] = v[j + 1];
        }
        v[7] = next;
    }

    /* After the loop, v[7] holds a(n). */
    return v[7];
}
