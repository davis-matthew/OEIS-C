/*
 * A267365  Decimal representation of the n-th iteration of the "Rule 126"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A267365: decimal representation of the n‑th
   iteration of Rule 126 elementary cellular automaton starting
   from a single ON cell.  */
/* The automaton is simulated iteratively; the resulting binary
   pattern of length 2*n+1 is interpreted as a binary number
   with the most significant bit first.  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* n=0 : a single ON cell -> binary "1" -> decimal 1 */
    if (n == 0) return (seq_t)1;

    /* Length of the automaton pattern after n steps is 2*n+1 */
    const size_t len = (size_t)2 * (size_t)n + 1;

    /* Allocate two temporary byte arrays for the current and next state. */
    unsigned char *cur = (unsigned char *)calloc(len, sizeof(unsigned char));
    unsigned char *tmp = (unsigned char *)calloc(len, sizeof(unsigned char));
    if (!cur || !tmp) {
        free(cur);
        free(tmp);
        return (seq_t)0;              /* allocation failure: return 0 */
    }

    /* Initial configuration: single ON cell in the middle */
    cur[(size_t)n] = 1;

    /* Rule 126 mapping: 111 -> 0, 000 -> 0, all other patterns -> 1.
       It can be represented by mask 0x7E (01111110 in binary). */
    const unsigned char rule_mask = 0x7E;

    for (idx_t step = 1; step <= n; ++step) {
        /* Apply Rule 126 to every cell */
        for (size_t i = 0; i < len; ++i) {
            unsigned left  = (i > 0)          ? cur[i - 1] : 0;
            unsigned mid   = cur[i];
            unsigned right = (i + 1 < len)    ? cur[i + 1] : 0;

            unsigned pat = (left << 2) | (mid << 1) | right;
            tmp[i] = (unsigned char)((rule_mask >> pat) & 1u);
        }
        /* Swap current and next state buffers */
        unsigned char *swap = cur;
        cur = tmp;
        tmp = swap;
    }

    /* Convert the final bit pattern into a decimal integer.
       The bits are processed from most significant to least
       significant (left to right). */
    seq_t result = 0;
    for (size_t i = 0; i < len; ++i) {
        result = result * 2 + cur[i];
    }

    free(cur);
    free(tmp);
    return result;
}
