/*
 * A281738  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 390", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A281738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 126, u64 n <= 126,
 *            u128 n <= 126, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A281738:  Decimal representation of the x‑axis of the
   n‑th stage of Rule 390 CA (von Neumann neighbourhood).
   The sequence satisfies the recurrence
      a(n) = a(n-2) – p(n)
   where the coefficients p(n) come from the numerator of the
   generating function given in the OEIS entry:
        P(x) = 12582912 x²⁵ + 4194304 x²⁴ – 10485760 x²³ – … – 3 x – 1
   For n > 25 we have p(n) = 0, hence a(n) = a(n‑2).  The values
   for 0 ≤ n ≤ 25 are obtained by the recurrence above, yielding
   a(24) = 1 and a(25) = 7, after which the sequence alternates 1, 7. */

seq_t a(idx_t n)
{
    /* Signed 32‑bit coefficients p[n] (for 0 ≤ n ≤ 25) from the
       generating function numerator.  Negative values mean that the
       term is added to a(n‑2); positive values are subtracted. */
    static const int32_t p[26] = {
        /*  n  coefficient  p[n]  (same sign as in P(x)) */
        -1,   /* 0   -1          */
        -3,   /* 1   -3          */
         0,   /* 2   0            */
        -4,   /* 3   -4          */
       -16,   /* 4   -16         */
       -32,   /* 5   -32         */
       -48,   /* 6   -48         */
      -160,   /* 7   -160        */
         0,   /* 8   0           */
      -256,   /* 9   -256        */
      -960,   /*10  -960         */
    -1600,   /*11 -1600         */
    -3072,   /*12 -3072         */
    -6144,   /*13 -6144         */
   -12288,   /*14 -12288        */
   -24576,   /*15 -24576        */
   -49152,   /*16 -49152        */
   -98304,   /*17 -98304        */
 -196608,   /*18 -196608       */
 -393216,   /*19 -393216       */
 -786432,   /*20 -786432       */
-1572864,   /*21 -1572864      */
-3145728,   /*22 -3145728      */
-10485760,  /*23 -10485760     */
 4194304,   /*24  +4194304     */
12582912    /*25  +12582912    */
    };

    /* Compute the first 26 terms once in a static array. */
    static seq_t values[26];
    static int initialized = 0;
    if (!initialized) {
        values[0] = (seq_t)1;          /* a(0) */
        values[1] = (seq_t)3;          /* a(1) */
        for (idx_t k = 2; k <= 25; ++k) {
            if (p[k] < 0) {
                /* a(k) = a(k-2) + |p(k)|  (add because p(k) is negative) */
                values[k] = values[k-2] + (seq_t)(-p[k]);
            } else {
                /* a(k) = a(k-2) – p(k)  (subtract because p(k) is positive) */
                values[k] = values[k-2] - (seq_t)p[k];
            }
        }
        initialized = 1;
    }

    /* For n > 25 the sequence repeats every two steps.          */
    if (n <= 25) {
        return values[n];
    } else {
        return (n & 1) ? values[25] : values[24];
    }
}
