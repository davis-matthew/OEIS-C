/*
 * A319364  Expansion of e.g.f. exp(x^3/3)/(1 - x).
 * https://oeis.org/A319364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) + (n-1)*(n-2)*a(n-3) - (n-1)*(n-2)*(n-3)*a(n-4)
 *            a(0) = 1, a(1) = 1, a(2) = 2, a(3) = 8
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    switch (n) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)1;
        case 2: return (seq_t)2;
        case 3: return (seq_t)8;
    }

    /* Circular buffer storing the five most recent terms:
       indices 0..4 contain a(k-4), a(k-3), a(k-2), a(k-1), a(k) respectively. */
    seq_t buf[5] = {(seq_t)1, (seq_t)1, (seq_t)2, (seq_t)8, (seq_t)0};

    for (idx_t k = 4; k <= n; ++k) {
        seq_t a_nm1 = buf[(k - 1) % 5];   /* a(k-1) */
        seq_t a_nm3 = buf[(k - 3) % 5];   /* a(k-3) */
        seq_t a_nm4 = buf[(k - 4) % 5];   /* a(k-4) */

        seq_t term1 = (seq_t)k * a_nm1;
        seq_t term2 = ((seq_t)(k - 1) * (seq_t)(k - 2)) * a_nm3;
        seq_t term3 = ((seq_t)(k - 1) * (seq_t)(k - 2) * (seq_t)(k - 3)) * a_nm4;

        buf[k % 5] = term1 + term2 - term3;
    }

    return buf[n % 5];
}
