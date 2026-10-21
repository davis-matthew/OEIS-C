/*
 * A287836  Number of words over the alphabet {0,1,...,10} such that no two
 *          consecutive terms have distance 5.
 * https://oeis.org/A287836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) + a(n-2) - 18*a(n-3) with initial values
 *            a(0)=1, a(1)=11, a(2)=109, a(3)=1081
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)11;
        case 2: return (seq_t)109;
        case 3: return (seq_t)1081;
        default:
            seq_t a0 = (seq_t)1;   /* a(n-3) */
            seq_t a1 = (seq_t)11;  /* a(n-2) */
            seq_t a2 = (seq_t)109; /* a(n-1) */
            seq_t a3 = (seq_t)1081;/* current a(n) */
            for (idx_t i = 4; i <= n; ++i) {
                seq_t next = (seq_t)10 * a3 + a2 - (seq_t)18 * a1;
                a0 = a1; a1 = a2; a2 = a3; a3 = next;
            }
            return a3;
    }
}
