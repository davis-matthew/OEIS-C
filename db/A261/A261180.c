/*
 * A261180  Flowsnake phases, exp(I 2 Pi a(n) / 6) are vectors in a sequence
 *          that visits points of the hexagonal root lattice A_2.
 * https://oeis.org/A261180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = phase of the nth symbol in the L‑system word generated
 *            from the start symbol P[0] using the substitution rules listed
 *            in OEIS A261180. The substitution expands each symbol into seven
 *            children; the nth symbol (1‑based) is found by expressing n-1 in
 *            base 7 and following the child choices. Th...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1; value for n=0 is unused */
        return (seq_t)0;

    idx_t idx   = n - 1;       /* 0‑based position */
    idx_t power = 1;
    /* find highest power of 7 ≤ idx */
    while (idx / power >= 7)
        power *= 7;

    int type = 0;   /* 0 = P, 1 = M */
    int sym  = 0;   /* index 0..2 */

    while (power > 0) {
        int digit = (int)(idx / power);
        idx     %= power;
        power   /= 7;

        int new_type, new_sym;
        if (type == 0) { /* P[n] */
            switch (digit) {
                case 0: new_type = 0; new_sym = sym;                   break;
                case 1: new_type = 1; new_sym = (sym + 2) % 3;          break;
                case 2: new_type = 1; new_sym = sym;                   break;
                case 3: new_type = 0; new_sym = (sym + 1) % 3;          break;
                case 4: new_type = 0; new_sym = sym;                   break;
                case 5: new_type = 0; new_sym = sym;                   break;
                case 6: new_type = 1; new_sym = (sym + 1) % 3;          break;
                default: new_type = 0; new_sym = sym;                  break;
            }
        } else { /* M[n] */
            switch (digit) {
                case 0: new_type = 0; new_sym = (sym + 1) % 3;          break;
                case 1: new_type = 1; new_sym = sym;                   break;
                case 2: new_type = 1; new_sym = sym;                   break;
                case 3: new_type = 1; new_sym = (sym + 1) % 3;          break;
                case 4: new_type = 0; new_sym = sym;                   break;
                case 5: new_type = 0; new_sym = (sym + 2) % 3;          break;
                case 6: new_type = 1; new_sym = sym;                   break;
                default: new_type = 0; new_sym = sym;                  break;
            }
        }

        type = new_type;
        sym  = new_sym;
    }

    int phase = (type == 0) ? (2 * sym) % 6 : (2 * sym + 3) % 6;
    return (seq_t)phase;
}
