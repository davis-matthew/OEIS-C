/*
 * A270087  Number of active (ON, black) cells in n-th stage of growth of
 *          two-dimensional cellular automaton defined by "Rule 73", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A270087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=4, a(2)=5, a(3)=40; For n>3: a(n)=0 if n even,
 *            a(n)=(2*n+1)^2 if n odd
 * Range:     u8 n <= 8, u16 n <= 128, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n) {
    case 0: return (seq_t)1;
    case 1: return (seq_t)4;
    case 2: return (seq_t)5;
    case 3: return (seq_t)40;
    default:
        if ((n & 1U) == 0)            /* even n > 3 */
            return (seq_t)0;
        else {                        /* odd n > 3 */
            seq_t nn = (seq_t)n;
            seq_t t  = 2 * nn + 1;   /* 2*n + 1 */
            return t * t;            /* (2*n + 1)^2 */
        }
    }
}
