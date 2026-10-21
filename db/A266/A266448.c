/*
 * A266448  Total number of ON (black) cells after n iterations of the "Rule
 *          25" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A266448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^2+2*n+(-1)^n*(7-2*n)+5)/4 for n>2. Special values:
 *            a(0)=1, a(1)=2, a(2)=4. Derived simpler equivalents: if n even
 *            (n>2): a(n) = (n^2 + 6)/2 -> 2*(n/2)^2 + 3 if n odd (n>2): a(n)
 *            = (n^2 + 2*n - 1)/2 -> 2*((n-1)/2)^2 + 4*((n-1)/2) + 1
 * Range:     u8 n <= 22, u16 n <= 362, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)2;
        case 2: return (seq_t)4;
        default:
            if (n & 1U) { /* odd n > 2 */
                idx_t k = (n - 1U) / 2U;   /* k = (n-1)/2 */
                seq_t kk = (seq_t)k;
                /* a = 2*kk*kk + 4*kk + 1 */
                return (seq_t)2U * kk * kk + (seq_t)4U * kk + (seq_t)1U;
            } else {            /* even n > 2 */
                idx_t k = n / 2U;          /* k = n/2 */
                seq_t kk = (seq_t)k;
                /* a = 2*kk*kk + 3 */
                return (seq_t)2U * kk * kk + (seq_t)3U;
            }
    }
}
