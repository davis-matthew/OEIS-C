/*
 * A286687  Positions of 1 in A286685; complement of A286686.
 * https://oeis.org/A286687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n - (b[n]==1 ? 1 : 0) where b is the Pell word (fixed
 *            point of the morphism 0→001, 1→0)
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static state: generated bits of the Pell word */
    static uint8_t *bits = NULL;   /* array of 0/1 */
    static idx_t bits_len = 0;     /* current number of bits */

    if (n == 0) {
        /* offset 1; behaviour for n=0 is irrelevant */
        return (seq_t)0;
    }

    /* generate bits until we have at least n of them */
    if (n > bits_len) {
        /* initialise if necessary */
        if (bits == NULL) {
            bits = (uint8_t *)malloc(1 * sizeof(uint8_t));
            if (bits == NULL) exit(1);  /* cannot happen in tests */
            bits[0] = 0;                /* start of the fixed point */
            bits_len = 1;
        }

        while (bits_len < n) {
            idx_t cur_len = bits_len;
            /* each 0 becomes 3 bits, each 1 becomes 1 bit,
               so the new length <= 3 * cur_len.     */
            uint8_t *new_bits = (uint8_t *)malloc(3 * cur_len * sizeof(uint8_t));
            if (new_bits == NULL) exit(1);
            idx_t pos = 0;
            for (idx_t i = 0; i < cur_len; ++i) {
                if (bits[i] == 0) {
                    new_bits[pos++] = 0;
                    new_bits[pos++] = 0;
                    new_bits[pos++] = 1;
                } else { /* bits[i] == 1 */
                    new_bits[pos++] = 0;
                }
            }
            free(bits);
            bits = new_bits;
            bits_len = pos;
        }
    }

    /* compute a(n) from the nth Pell bit */
    uint8_t pn = bits[n - 1];
    seq_t val = (seq_t)n * 2;
    if (pn == 1)
        val -= 1;
    return val;
}
