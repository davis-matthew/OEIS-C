/*
 * A389234  Number of mutual-visibility sets in the n-double cone graph.
 * https://oeis.org/A389234
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the generating function:
 * a(n) - 6*a(n-1) + 9*a(n-2) - 4*a(n-3) = p(n),
 * where
 *   p(3)= 79,  p(4)=-189,  p(5)= 66,
 *   p(6)=-4,   p(7)= 50,   p(8)=-4,
 *   p(9)=-16   and p(n)=0 for n<3 or n>9.
 * Initial values: a(0)=a(1)=a(2)=0.
 * This yields the OEIS sequence A389234 for all n ≥ 3. */
seq_t a(idx_t n)
{
    seq_t a0 = 0;  /* a(0) */
    seq_t a1 = 0;  /* a(1) */
    seq_t a2 = 0;  /* a(2) */
    seq_t val;

    if (n < 3)
        return 0;          /* 0 is not part of the sequence, but defined for safety */

    for (idx_t i = 3; i <= n; ++i) {
        int p = 0;
        switch (i) {
            case 3: p = 79;   break;
            case 4: p = -189; break;
            case 5: p = 66;   break;
            case 6: p = -4;   break;
            case 7: p = 50;   break;
            case 8: p = -4;   break;
            case 9: p = -16;  break;
            default: p = 0;   break;
        }

        /* a(n) = 6*a(n-1) - 9*a(n-2) + 4*a(n-3) + p(n) */
        val = 6 * a2;
        val -= 9 * a1;
        val += 4 * a0;
        if (p < 0)
            val -= (seq_t)(-p);
        else
            val += (seq_t)p;

        /* shift the window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = val;
    }
    return val;
}
