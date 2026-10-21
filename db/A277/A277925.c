/*
 * A277925  Odd squares and 8 times odd squares, sorted.
 * https://oeis.org/A277925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 172, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: sorted merge of odd squares (2k-1)^2 and 8*(2k-1)^2. */
seq_t a(idx_t n)
{
    idx_t i1 = 1;        /* index for odd squares          */
    idx_t i2 = 1;        /* index for 8×odd squares        */
    seq_t v1 = 1;        /* (2*1-1)^2 = 1                  */
    seq_t v2 = (seq_t)8; /* 8*(2*1-1)^2 = 8                */
    seq_t last = 0;
    idx_t cnt  = 0;

    while (cnt < n)
    {
        if (v1 <= v2)                /* take the smaller value   */
        {
            last = v1;
            ++i1;
            seq_t t = (seq_t)(2 * i1 - 1);
            v1 = t * t;             /* next odd square          */
        }
        else
        {
            last = v2;
            ++i2;
            seq_t t = (seq_t)(2 * i2 - 1);
            v2 = (seq_t)8 * t * t;   /* next 8×odd square        */
        }
        ++cnt;
    }
    return last;
}
