/*
 * A280531  a(n) = A049501(A000142(n)).
 * https://oeis.org/A280531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = major index of the binary representation of n!
 * Range:     u8 n <= 19, u16 n <= 149, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 9890 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Compute the major index of a binary string.
   The binary string is stored in big‑endian order, most significant word first. */
static seq_t major_index_of_binary(uint64_t *words, size_t sz)
{
    /* Find the most significant word and the number of bits it uses */
    size_t msw = sz - 1;
    while (msw > 0 && words[msw] == 0)
        --msw;
    uint64_t high_word = words[msw];
    int high_bits = 64 - __builtin_clzll(high_word); /* words[msw] != 0 */

    seq_t sum = 0;
    uint64_t pos = 1;      /* position of the current bit (1‑based) */
    uint64_t prev_bit = 0; /* bit just < processed bit */
    int first = 1;         /* no descent check for the first bit */

    for (int w = (int)msw; w >= 0; --w) {
        uint64_t word = words[w];
        int start_bit = (w == (int)msw) ? high_bits - 1 : 63;
        for (int b = start_bit; b >= 0; --b) {
            uint64_t bit = (word >> b) & 1ULL;
            if (!first) {
                if (prev_bit && !bit)    /* transition 1 → 0 */
                    sum += (seq_t)(pos - 1); /* position of the preceding bit */
            } else
                first = 0;
            prev_bit = bit;
            ++pos;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* n == 0 or n == 1 : factorial = 1, binary "1", major index = 0 */
    if (n <= 1)
        return 0;

    /* Dynamically grow a little‑endian array of 64‑bit words holding the factorial. */
    size_t cap = 1;
    size_t sz  = 1;
    uint64_t *words = (uint64_t*)malloc(cap * sizeof(uint64_t));
    words[0] = 1;  /* 0! = 1 */

    for (idx_t i = 2; i <= n; ++i) {
        uint64_t carry = 0;
        for (size_t j = 0; j < sz; ++j) {
            unsigned __int128 prod = (unsigned __int128)words[j] * i + carry;
            words[j] = (uint64_t)prod;
            carry   = (uint64_t)(prod >> 64);
        }
        if (carry) {
            if (sz == cap) {
                cap = cap ? cap * 2 : 1;
                words = (uint64_t*)realloc(words, cap * sizeof(uint64_t));
            }
            words[sz++] = carry;
            carry = 0;
        }
    }

    seq_t result = major_index_of_binary(words, sz);
    free(words);
    return result;
}
