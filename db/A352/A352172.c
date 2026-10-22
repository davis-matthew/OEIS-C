/*
 * A352172  a(n) is the product of the cubes of the nonzero digits of n.
 * https://oeis.org/A352172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ∏_{d∈digits(n), d>0} d^3
 * Range:     u8 n <= 6, u16 n <= 58, u32 n <= 3798, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A352172: product of the cubes of the non‑zero digits of n.
   n is indexed from 1 (a(1)=1).  The function works for any build that defines
   seq_t and idx_t as unsigned integer types. */
seq_t a(idx_t n) {
    /* Use a variable of type seq_t to store the result so that all
       intermediate products stay within seq_t bounds. */
    seq_t prod = 1;
    /* Iterate over the decimal digits of n.  Skip zero digits. */
    while (n > 0) {
        unsigned d = (unsigned)(n % 10);
        if (d != 0) {
            seq_t cube = (seq_t)d * (seq_t)d * (seq_t)d;
            prod *= cube;
        }
        n /= 10;
    }
    return prod;
}
