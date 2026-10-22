/*
 * A329781  Numbers that are congruent to {+-1, +-3, +-4, +-5, +-9, +-10,
 *          +-11, +-14, +-15, +-16, +-17, +-19} (mod 40).
 * https://oeis.org/A329781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th positive integer such that a(n) mod 40 is in
 *            {1,3,4,5,9,10,11,14,15,16,17,19,
 *            21,23,24,25,26,29,30,31,35,36,37,39}. The sequence starts
 *            a(1)=1, a(2)=3, a(3)=4, ... as given in OEIS
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uintptr_t etc.  Not strictly needed, but harmless */

seq_t a(idx_t n)
{
    /* Handle out‑of‑range input conservatively */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Residue table: 1 if the residue 'i' is allowed, 0 otherwise. */
    static const unsigned char allowed[40] = {
        0,1,0,1,1,1,0,0,0,1,
        1,1,0,0,1,1,1,1,0,1,
        0,1,0,1,1,1,1,0,0,1,
        1,1,0,0,0,1,1,1,0,1
    };

    seq_t count = 0;   /* Number of allowed integers found so far */
    seq_t num   = 0;   /* Current integer being tested */

    for (;;) {
        ++num;                           /* Next integer */
        unsigned r = (unsigned)(num % 40);/* Residue modulo 40 */
        if (allowed[r]) {                 /* Is it in the allowed set? */
            ++count;
            if (count == n) {
                return num;
            }
        }
    }
}
