/*
 * A262997  a(n+3) = a(n) + 24*n + 40, a(0)=0, a(1)=5, a(2)=19.
 * https://oeis.org/A262997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+3) = a(n) + 24*n + 40, a(0)=0, a(1)=5, a(2)=19. Closed‑form:
 *            let m = ⌊n/3⌋ and r = n mod 3. a(3m) = 36*m*m + 4*m a(3m+1) =
 *            36*m*m + 28*m + 5 a(3m+2) = 36*m*m + 52*m + 19
 * Range:     u8 n <= 7, u16 n <= 127, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)(n / 3);
    seq_t r = (seq_t)(n % 3);

    if (r == 0u) {
        return 36 * m * m + 4 * m;
    } else if (r == 1u) {
        return 36 * m * m + 28 * m + 5;
    } else { /* r == 2 */
        return 36 * m * m + 52 * m + 19;
    }
}
