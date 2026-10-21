/*
 * A272851  Number of distinct nonzero Fibonacci numbers among the contiguous
 *          substrings of the binary digits of n.
 * https://oeis.org/A272851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   init_fib_table
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Custom: Count distinct nonzero Fibonacci numbers appearing as contiguous binary substrings of n */
/* Sequence: a(n) = number of distinct non‑zero Fibonacci numbers in all contiguous substrings of binary(n) */

#include <stdbool.h>
#include <string.h>
#include "../utils/init_fib_table.h"

seq_t a(idx_t n) {
    /* Precompute Fibonacci indicators up to 2^15 - 1 (enough for n <= 10000). */
    static bool fib_table[1 << 15] = {0};
    static bool fib_init = false;
    if (!fib_init) {
        init_fib_table(fib_table);
        fib_init = true;
    }

    /* Convert n to binary digits (MSB first). */
    uint64_t tmp = n;
    uint8_t bits[64] = {0};
    int len = 0;
    while (tmp > 0) {
        bits[len++] = tmp & 1;
        tmp >>= 1;
    }
    if (len == 0) bits[len++] = 0;           /* n == 0 case, though not used. */
    uint8_t msb[64];
    for (int i = 0; i < len; i++) msb[i] = bits[len - 1 - i];

    /* Mark seen Fibonacci values for this n. */
    static bool seen[1 << 15];
    memset(seen, 0, sizeof(seen));

    seq_t count = 0;
    for (int i = 0; i < len; i++) {
        uint64_t val = 0;
        for (int j = i; j < len; j++) {
            val = (val << 1) | msb[j];
            if (val == 0) continue;                 /* skip zero substrings */
            if (val < (1u << 15) && fib_table[val]) {
                if (!seen[val]) {
                    seen[val] = true;
                    count++;
                }
            }
        }
    }
    return count;
}
