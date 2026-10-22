/*
 * A360506  Read A360505(n) as if it were a base-3 string and write it in base
 *          10.
 * https://oeis.org/A360506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A028898(A360505(n)) A360505(n) is the string consisting
 *            of the base‑3 representations of the integers n, n‑1, …, 1. a(n)
 *            is that string interpreted as a base‑3 number. The routine
 *            builds the value incrementally: start with v = 0 and p = 1 (=
 *            3^0). For each i from 1 to n: v = i * p + v p *=...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t v = 0;
    seq_t p = 1;          /* p = 3^0 */

    for (idx_t i = 1; i <= n; ++i) {
        /* number of base‑3 digits of i */
        idx_t t = i;
        int d = 0;
        do {
            ++d;
            t /= 3;
        } while (t);

        /* prepend i: v = i * 3^previous_length + v */
        v = (seq_t)i * p + v;

        /* update p = p * 3^d for the next iteration */
        for (int k = 0; k < d; ++k) p *= (seq_t)3;
    }
    return v;
}
