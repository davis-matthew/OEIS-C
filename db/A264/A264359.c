/*
 * A264359  Number of (n+1)X(3+1) arrays of permutations of 0..n*4+3 with each
 *          element having directed index change 0,0 0,2 1,0 or -1,-2.
 * https://oeis.org/A264359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A direct implementation of the recurrence for OEIS A264359:
 *   a(n) = 3*a(n-1) + 7*a(n-2) + 3*a(n-3) - 5*a(n-4) + 3*a(n-5) - a(n-6)
 *   (n >= 7), with initial values:
 *     a(1)=9, a(2)=36, a(3)=169, a(4)=784, a(5)=3600, a(6)=16641.
 *   The index is 1‑based; a(0) is undefined but will return 0 for safety.
 */

seq_t a(idx_t n)
{
    /* Return 0 for the undefined index 0.  The sequence is defined for n>=1. */
    if (n == 0) return (seq_t)0;

    /* Initial six terms (index 0 unused). */
    static const seq_t init[7] = {
        (seq_t)0,          /* 0 – unused */
        (seq_t)9,          /* 1 */
        (seq_t)36,         /* 2 */
        (seq_t)169,        /* 3 */
        (seq_t)784,        /* 4 */
        (seq_t)3600,       /* 5 */
        (seq_t)16641       /* 6 */
    };

    /* Return the pre‑computed initial value if requested. */
    if (n <= 6) return init[n];

    /* Sliding window of the last six terms.
       t[0] = a(k-6), t[1] = a(k-5), ..., t[5] = a(k-1) for current k.   */
    seq_t t[6];
    t[0] = init[1];
    t[1] = init[2];
    t[2] = init[3];
    t[3] = init[4];
    t[4] = init[5];
    t[5] = init[6];

    idx_t i;
    for (i = 7; i <= n; ++i) {
        /* Apply the recurrence precisely: */
        seq_t next =
            (seq_t)3 * t[5] +   /* 3*a(k-1) */
            (seq_t)7 * t[4] +   /* 7*a(k-2) */
            (seq_t)3 * t[3] +   /* 3*a(k-3) */
            (seq_t)5 * t[2] * -1 + /* -5*a(k-4) */
            (seq_t)3 * t[1] +   /* 3*a(k-5) */
            (seq_t)1 * t[0] * -1; /* -a(k-6) */

        /* Shift the window for the next iteration. */
        t[0] = t[1];
        t[1] = t[2];
        t[2] = t[3];
        t[3] = t[4];
        t[4] = t[5];
        t[5] = next;
    }

    /* t[5] now holds a(n). */
    return t[5];
}
