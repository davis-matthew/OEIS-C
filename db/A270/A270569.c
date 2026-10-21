/*
 * A270569  Number of active (ON, black) cells in n-th stage of growth of
 *          two-dimensional cellular automaton defined by "Rule 323", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A270569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(1) = 5 a(2) = 5 a(4) = 9 a(n) = 2*n + 5 for even n ≥
 *            6 a(n) = 4*n*n - 2*(n-3) for odd n ≥ 3
 * Range:     u8 n <= 8, u16 n <= 128, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)5;
    if (n == 4) return (seq_t)9;

    if ((n & 1U) && n >= 3) {
        seq_t nn = (seq_t)n;
        seq_t val = nn * nn;                  /* n² */
        val = val * (seq_t)4;                 /* 4·n² */
        val = val - (seq_t)2 * ((seq_t)n - (seq_t)3);
        return val;
    }

    if ((n & 1U) == 0) {
        /* even n ≥ 6 */
        seq_t val = (seq_t)n * (seq_t)2;     /* 2·n */
        val = val + (seq_t)5;
        return val;
    }

    /* Fallback: should not be reached */
    return (seq_t)0;
}
