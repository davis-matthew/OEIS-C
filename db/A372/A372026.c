/*
 * A372026  Minimum second Zagreb index of maximal 2-degenerate graphs with n
 *          vertices.
 * https://oeis.org/A372026
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 32*n - 110 for n > 8; a(3)=12, a(4)=33, a(5)=51, a(6)=86,
 *            a(7)=116, a(8)=147
 * Range:     u8 n <= 11, u16 n <= 2051, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Undefined behaviour for n < 3 – return 0 for safety */
    if (n < 3)
        return 0;

    /* Explicit tiny table for n = 3 … 8 */
    if (n <= 8)
    {
        switch ((idx_t)n)
        {
            case 3: return (seq_t)12;
            case 4: return (seq_t)33;
            case 5: return (seq_t)51;
            case 6: return (seq_t)86;
            case 7: return (seq_t)116;
            case 8: return (seq_t)147;
            default: return 0; /* should never reach here */
        }
    }

    /* Linear formula for n > 8 */
    seq_t nn = (seq_t)n;
    return (seq_t)32 * nn - (seq_t)110;
}
