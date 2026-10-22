/*
 * A358773  a(n) = 1 if the arithmetic derivative of n is of the form 4k+3,
 *          otherwise 0.
 * https://oeis.org/A358773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the arithmetic derivative of n is congruent to 3
 *            (mod 4), otherwise a(n) = 0. The arithmetic derivative is
 *            defined by d(n) = Σ e·n/p for each prime power p^e dividing n
 * Range:     u8 n <= 342, u16 n <= 9203, u32 n <= 9203, u64 n <= 9203,
 *            u128 n <= 9203, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1u)                 /* n = 0 or 1 → derivative 0 */
        return 0;

    idx_t nn   = n;              /* mutable copy of the number */
    seq_t mod4 = 0;              /* derivative modulo 4 */

    idx_t p = 2;
    while (p * p <= nn) {
        if (nn % p == 0u) {
            idx_t e = 0;
            while (nn % p == 0u) { nn /= p; e++; }

            /* compute term modulo 4, avoiding overflow */
            seq_t part1 = (((seq_t)n) / ((seq_t)p)) & 3u;
            seq_t part2 = (seq_t)(e & 3u);
            seq_t term  = (part1 * part2) & 3u;

            mod4 = (mod4 + term) & 3u;
        }
        p++;
    }

    if (nn > 1u) {                /* remaining prime factor */
        seq_t part1 = (((seq_t)n) / ((seq_t)nn)) & 3u;
        seq_t term  = part1;     /* e = 1 */
        mod4 = (mod4 + term) & 3u;
    }

    return (mod4 == 3u) ? (seq_t)1 : (seq_t)0;
}
