/*
 * A301722  Coordination sequence for node of type V2 in "krb" 2-D tiling (or
 *          net).
 * https://oeis.org/A301722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n = 0 5 for n = 1 10 for n = 2 15 for n = 3 15 +
 *            16*⌊(n-3)/3⌋ + {0,7,12}[ (n-3) mod 3 ] for n ≥ 4 This implements
 *            the pattern of successive differences 7,5,4 repeating after the
 *            3rd term
 * Range:     u8 n <= 48, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Directly return the first four terms. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)10;
    if (n == 3) return (seq_t)15;

    /* For n ≥ 4, compute using the closed‑form expression. */
    idx_t t   = n - 3;              /* non‑negative */
    seq_t m   = (seq_t)(t / 3);     /* ⌊(n-3)/3⌋            */
    seq_t r   = (seq_t)(t % 3);     /* (n-3) mod 3          */
    seq_t res = (seq_t)15 + (seq_t)16 * m;

    /* Add the partial sum for the remainder r. */
    if (r == 1) {
        res += (seq_t)7;
    } else if (r == 2) {
        res += (seq_t)12;
    }
    /* r == 0 contributes nothing. */

    return res;
}
