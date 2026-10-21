/*
 * A241249  Number of nX2 0..3 arrays with no element equal to one plus the
 *          sum of elements to its left or zero plus the sum of elements above
 *          it or one plus the sum of the elements diagonally to its northwest
 *          or zero plus the sum of the elements antidiagonally to its
 *          northeast, modulo 4.
 * https://oeis.org/A241249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts n×2 arrays over {0,1,2,3} whose entries satisfy the
 *            following rule for every cell (i,j): Let L be the sum of
 *            elements to the left of (i,j) (0 if none). Let U be the sum of
 *            elements above (i,j) (0 if none). Let NW be the sum of elements
 *            diagonally northwest of (i,j) (0 if none). Let NE...
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 26, u64 n <= 52, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for size_t if needed */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* State: [sum1][sum2][preva][prevb] -> count.
       sum1, sum2 are cumulative sums modulo 4 of columns 1 and 2
       up to the preceding row.
       preva, prevb are the values of columns 1 and 2 in the preceding row.
       Encode as ( (sum1*4+sum2)*16 + preva*4 + prevb ).
    */
    static seq_t curr[256];
    static seq_t next[256];

    /* initialise state for 0 rows: sums 0, previous values 0. */
    for (int i = 0; i < 256; ++i)
        curr[i] = 0;
    curr[0] = 1;   /* index 0 corresponds to (0,0,0,0) */

    for (idx_t row = 0; row < n; ++row)
    {
        /* zero next state */
        for (int i = 0; i < 256; ++i)
            next[i] = 0;

        for (int sum1 = 0; sum1 < 4; ++sum1)
        {
            for (int sum2 = 0; sum2 < 4; ++sum2)
            {
                for (int preva = 0; preva < 4; ++preva)
                {
                    for (int prevb = 0; prevb < 4; ++prevb)
                    {
                        int idx0 = ((((sum1 * 4) + sum2) * 16) + (preva * 4) + prevb);
                        seq_t count = curr[idx0];
                        if (!count)
                            continue;

                        for (int a = 0; a < 4; ++a)
                        {
                            /* For left column: forbid 1, sum1, prevb */
                            if (a == 1 || a == sum1 || a == prevb)
                                continue;

                            for (int b = 0; b < 4; ++b)
                            {
                                /* For right column: forbid
                                   (1+a)%4, sum2, (1+preva)%4, and 0          */
                                if (b == ((1 + a) & 3))
                                    continue;
                                if (b == sum2)
                                    continue;
                                if (b == ((1 + preva) & 3))
                                    continue;
                                if (b == 0)
                                    continue;

                                int ns1 = (sum1 + a) & 3;
                                int ns2 = (sum2 + b) & 3;
                                int idx1 = ((((ns1 * 4) + ns2) * 16) + (a * 4) + b);
                                next[idx1] += count;
                            }
                        }
                    }
                }
            }
        }

        /* swap curr and next */
        for (int i = 0; i < 256; ++i)
            curr[i] = next[i];
    }

    /* sum all counts for final states */
    seq_t total = 0;
    for (int i = 0; i < 256; ++i)
        total += curr[i];

    return total;
}
