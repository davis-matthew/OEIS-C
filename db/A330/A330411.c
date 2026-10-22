/*
 * A330411  a(n) is the index of the first 0 term in the rumor sequence with
 *          initial 0th term 1 and parameters b = 3 and n.
 * https://oeis.org/A330411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the index of the first zero in the rumor sequence
 * with parameters b = 3 and initial value z[0] = 1.
 * That is,
 *   a(n) = inf{ m>0 | z[0] = 1, z[m] = 0,
 *                z[k] = (3*z[k-1] mod (k + n)) for k = 1..m }.
 */
seq_t a(idx_t n)
{
    /* Initial term */
    seq_t z = 1;

    /* Iterate k = 1, 2, 3, ... until we hit zero */
    for (idx_t k = 1; ; ++k) {
        /* modulus is (k + n); it fits in an idx_t */
        idx_t mod = k + n;
        seq_t m = (seq_t)mod;

        /* Compute z = (3 * z) mod m */
        seq_t prod = z * 3;
        z = prod % m;

        if (z == 0)
            return k;          /* first zero found */
    }
}
