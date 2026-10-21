/*
 * A296209  a(n) = 1 if n is a pentanacci number, 0 otherwise; characteristic
 *          function for A001591.
 * https://oeis.org/A296209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 iff n is a pentanacci number, 0 otherwise
 * Range:     u8 n <= 5099, u16 n <= 9995, u32 n <= 9995, u64 n <= 9995,
 *            u128 n <= 9995, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t type for comparisons */
    seq_t sn = (seq_t)n;

    /* The first five pentanacci numbers */
    seq_t vals[5];
    vals[0] = (seq_t)0;
    vals[1] = (seq_t)0;
    vals[2] = (seq_t)0;
    vals[3] = (seq_t)0;
    vals[4] = (seq_t)1;

    /* Check the initial values */
    for (int i = 0; i < 5; ++i) {
        if (vals[i] == sn) return (seq_t)1;
    }

    /* Generate subsequent terms until the value exceeds sn */
    while (1) {
        seq_t next = vals[4] + vals[3] + vals[2] + vals[1] + vals[0];
        if (next > sn) break;
        if (next == sn) return (seq_t)1;
        /* Shift the window */
        vals[0] = vals[1];
        vals[1] = vals[2];
        vals[2] = vals[3];
        vals[3] = vals[4];
        vals[4] = next;
    }
    return (seq_t)0;
}
