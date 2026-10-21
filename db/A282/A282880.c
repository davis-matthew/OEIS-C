/*
 * A282880  Number of nX3 0..1 arrays with no 1 equal to more than one of its
 *          horizontal, vertical and antidiagonal neighbors, with the
 *          exception of exactly one element.
 * https://oeis.org/A282880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula from the OEIS entry:
   a(1) = 1
   a(2) = 8
   a(3) = 74
   a(4) = 430
   a(5) = 2426
   a(6) = 13062
   a(7) = 67676
   a(8) = 342972
   a(9) = 1707597
   a(10) = 8384136
   a(11) = 40716024
   a(12) = 195950228
   a(13) = 935955604
   a(14) = 4442192472
   a(15) = 20968437076
   a(16) = 98509310972
   a(17) = 460879910601
   a(18) = 2148369624844
   a(19) = 9981992555058
   a(20) = 46244594782978
   a(21) = 213681269956154
   a(22) = 985012878231418
   For n ≥ 23:
   a(n) = 2*a(n‑1) +17*a(n‑2) +22*a(n‑3) -75*a(n‑4) -376*a(n‑5)
          -834*a(n‑6) -1206*a(n‑7) -1229*a(n‑8) -824*a(n‑9) -187*a(n‑10)
          +346*a(n‑11) +511*a(n‑12) +344*a(n‑13) +91*a(n‑14)
          -74*a(n‑15) -106*a(n‑16) -42*a(n‑17) +2*a(n‑18) +16*a(n‑19)
          +4*a(n‑20) -a(n‑22)
   (the coefficient of a(n‑21) is implicitly zero). */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* initial 22 terms */
    static const seq_t init[22] = {
        (seq_t)1,      (seq_t)8,      (seq_t)74,      (seq_t)430,
        (seq_t)2426,   (seq_t)13062,  (seq_t)67676,   (seq_t)342972,
        (seq_t)1707597,(seq_t)8384136,(seq_t)40716024, (seq_t)195950228,
        (seq_t)935955604,(seq_t)4442192472,(seq_t)20968437076,
        (seq_t)98509310972,(seq_t)460879910601,(seq_t)2148369624844,
        (seq_t)9981992555058,(seq_t)46244594782978,
        (seq_t)213681269956154,(seq_t)985012878231418
    };

    /* coefficients for offsets 1..22, (-a(n-21) coefficient omitted, i.e. 0) */
    static const int coeff[22] = {
        /* offset 1  */  -1, /* a(n-22) */
        /* offset 2  */  0,  /* a(n-21) (implicit 0) */
        2, 17, 22, -75, -376, -834, -1206, -1229,
        -824, -187, 346, 511, 344, 91,
        -74, -106, -42, 2, 16, 4
    };
    /* Note: coeff array is indexed by offset from 13 to 1 (reverse order) to
       match the description above.  The last 13 entries correspond to
       a(n-1) .. a(n-13), the preceding 3 to a(n-16)..a(n-18),
       and the first two to a(n-21) (0) and a(n-22) (-1).  */
    /* Reorder the coefficient array to match the jump offsets from n-1 to n-22 */
    static const int reorder[22] = {
        2, 17, 22, -75, -376, -834, -1206, -1229,
        -824, -187, 346, 511, 344, 91,
        -74, -106, -42, 2, 16, 4, 0, -1
    };

    if (n <= 22)
        return init[(size_t)n - 1];

    seq_t vals[22];
    for (size_t i = 0; i < 22; ++i)
        vals[i] = init[i];

    for (idx_t i = 23; i <= n; ++i)
    {
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;
        for (int k = 0; k < 22; ++k)
        {
            int c = reorder[k];
            seq_t v = vals[21 - k]; /* vals[21] is a(n-1), vals[0] is a(n-22) */
            if (c > 0)
                sum_pos += v * (seq_t)c;
            else
                sum_neg += v * (seq_t)(-c);
        }
        seq_t next = sum_pos - sum_neg;

        /* shift left by one: vals[0] becomes vals[1], ..., vals[20] becomes vals[21] */
        for (int j = 0; j < 21; ++j)
            vals[j] = vals[j + 1];
        vals[21] = next;
    }

    return vals[21];
}
