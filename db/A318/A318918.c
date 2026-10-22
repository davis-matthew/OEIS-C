/*
 * A318918  Denominators of power series solution to differential equation
 *          diff(diff(y(t),t),t)+t*y(t) = 0, with initial conditions y(0)=1,
 *          Dy(0)=1/2.
 * https://oeis.org/A318918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n ≡ 2 (mod 3); a(n+3) = (n+2)(n+3)·a(n) otherwise
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 17, u64 n <= 27, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base values for n = 0, 1, 2 */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)1;

    /* Working values for each residue class modulo 3 */
    seq_t vals[3];
    vals[0] = (seq_t)1;  /* a(0) */
    vals[1] = (seq_t)2;  /* a(1) */
    vals[2] = (seq_t)1;  /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        idx_t mod = i % 3;
        if (mod == 2) {                /* a(i) = 1 for n ≡ 2 (mod 3) */
            vals[2] = (seq_t)1;
        } else {
            idx_t prev = i - 3;          /* index of the value we multiply by */
            seq_t factor = (seq_t)(prev + 2) * (seq_t)(prev + 3);
            if (mod == 0) {
                vals[0] = factor * vals[0];
            } else { /* mod == 1 */
                vals[1] = factor * vals[1];
            }
        }
    }

    idx_t mod = n % 3;
    return vals[mod];
}
