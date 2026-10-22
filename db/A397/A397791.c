/*
 * A397791  Numbers k with the property that for every two Dirichlet
 *          characters chi and chi' modulo k with ker(chi) = ker(chi'), we
 *          have chi = chi' or chi^(-1) = chi'.
 * https://oeis.org/A397791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A397791: Numbers k such that for any two Dirichlet characters χ and χ′ modulo k 
   with ker(χ) = ker(χ′) we have χ = χ′ or χ = χ′⁻¹.
   The set of such k is finite and consists of the 30 integers below. */

seq_t a(idx_t n) {
    /* Static array of all terms of the sequence in increasing order. */
    static const seq_t seq[30] = {
        1,   2,   3,   4,   5,   6,   7,   8,   9,  10,
       12,  14,  15,  16,  18,  20,  21,  24,  28,  30,
       36,  40,  42,  48,  56,  60,  72,  84, 120, 168
    };
    /* 1‑based indexing; return 0 if n is out of bounds. */
    if (n < 1 || n > 30) return 0;
    return seq[n - 1];
}
