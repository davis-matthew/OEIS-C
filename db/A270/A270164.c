/*
 * A270164  Number of active (ON, black) cells in n-th stage of growth of
 *          two-dimensional cellular automaton defined by "Rule 107", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A270164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   for n > 7, a(n) = (n^2 + 2n)/2 if n is even a(n) = (7n^2 + 8n +
 *            3)/2 if n is odd Initial values: a(0)=1, a(1)=5, a(2)=5,
 *            a(3)=41, a(4)=12, a(5)=109, a(6)=16, a(7)=197
 * Range:     u8 n <= 8, u16 n <= 128, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first eight terms exactly */
    switch (n) {
        case 0: return 1;
        case 1: return 5;
        case 2: return 5;
        case 3: return 41;
        case 4: return 12;
        case 5: return 109;
        case 6: return 16;
        case 7: return 197;
        default: break;
    }

    /* for n > 7 use the closed form, separating even and odd n */
    if ((n & (idx_t)1) == 0) {   /* n is even */
        seq_t nn = (seq_t)n;          /* cast index to seq_t first           */
        return (nn * (nn + 2)) / 2;  /* (n^2 + 2n) / 2                     */
    } else {                        /* n is odd  */
        seq_t nn = (seq_t)n;          /* cast index to seq_t first           */
        return ( ( (seq_t)7 * nn * nn ) + ( (seq_t)8 * nn ) + (seq_t)3 ) / 2;
        /* (7n^2 + 8n + 3) / 2                 */
    }
}
