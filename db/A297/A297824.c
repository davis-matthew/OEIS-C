/*
 * A297824  The number of iterations to remove all runs from the binary string
 *          11011100...n (formed by concatenating the first n binary numbers,
 *          see A058935(n)).
 * https://oeis.org/A297824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A297824: the number of iterations
   required to remove all runs of length ≥ 2 from the binary string
   formed by concatenating the first n binary numbers 1, 2, 3, …, n.
   The function works for all n ≥ 1 and is O(n log n + k·L),
   where L is the length of the string and k is the number of
   iterations (k is small for n ≤ 10 000). */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* ----------------------------------------------------------
       Build the binary string 11011100...n
       ---------------------------------------------------------- */
    size_t bits_len  = 0;   /* actual number of bits in the string */
    size_t bits_cap  = 0;   /* allocated capacity (bytes) */
    uint8_t *bits    = NULL;/* array holding the bits (0 or 1) */

    for (idx_t num = 1; num <= n; ++num) {
        /* number of bits of 'num' (without leading zeros) */
        size_t bitlen = 0;
        uint64_t v = (uint64_t)num;
        while (v) { ++bitlen; v >>= 1; }
        if (bitlen == 0) bitlen = 1;   /* safeguard for num = 0, never used */

        /* ensure enough storage */
        if (bits_len + bitlen > bits_cap) {
            size_t new_cap = bits_cap ? bits_cap << 1 : 8;
            while (new_cap < bits_len + bitlen) new_cap <<= 1;
            uint8_t *tmp = (uint8_t *)realloc(bits, new_cap);
            if (!tmp) { free(bits); return 0; }
            bits = tmp;
            bits_cap = new_cap;
        }

        /* append bits from most significant to least significant */
        for (size_t k = 0; k < bitlen; ++k) {
            uint8_t bit = (uint8_t)(((num >> (bitlen - 1 - k)) & 1u));
            bits[bits_len++] = bit;
        }
    }

    /* ----------------------------------------------------------
       Iteratively remove all runs of length ≥ 2
       ---------------------------------------------------------- */
    seq_t iter = 0;           /* iteration counter */

    while (1) {
        /* Build next string after removing runs >= 2 */
        uint8_t *next = (uint8_t *)malloc(bits_len); /* maximum possible size */
        if (!next) { free(bits); return 0; }

        size_t next_len = 0;
        int removed = 0;          /* flag whether any run was removed */

        for (size_t i = 0; i < bits_len; ) {
            size_t j = i;
            while (j < bits_len && bits[j] == bits[i]) ++j;
            size_t runlen = j - i;

            if (runlen == 1) {              /* keep isolated bit */
                next[next_len++] = bits[i];
            } else {                         /* run removed */
                removed = 1;
            }
            i = j;
        }

        if (!removed) {                    /* no more runs >= 2 */
            free(bits);
            free(next);
            break;
        }

        free(bits);
        bits = next;
        bits_len = next_len;
        ++iter;
    }

    return iter;
}
