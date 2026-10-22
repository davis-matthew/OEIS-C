/*
 * A380451  Number of disjoint-path coverings for 2 X n rectangular grids,
 *          admitting zero-length paths.
 * https://oeis.org/A380451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 4*a(n-2) - a(n-3) + a(n-4),
 *            a(1)=2,a(2)=15,a(3)=95,a(4)=604
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n) {
        case 1: return (seq_t)2;
        case 2: return (seq_t)15;
        case 3: return (seq_t)95;
        case 4: return (seq_t)604;
        default: break;
    }

    seq_t a1 = (seq_t)2;   /* a(n-4) */
    seq_t a2 = (seq_t)15;  /* a(n-3) */
    seq_t a3 = (seq_t)95;  /* a(n-2) */
    seq_t a4 = (seq_t)604; /* a(n-1) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t new_term = (seq_t)7 * a4 + a1;
        new_term -= (seq_t)4 * a3;
        new_term -= a2;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = new_term;
    }
    return a4;
}
