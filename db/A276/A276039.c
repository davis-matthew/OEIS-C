/*
 * A276039  Numbers using only digits 1 and 7.
 * https://oeis.org/A276039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 46, u32 n <= 1534, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  Formula:  a(n) is the n-th positive integer (in ascending order) whose
 *  decimal representation uses only the digits 1 and 7.  The n-th term is
 *  obtained by treating the index of the number within its fixed length
 *  group as a binary number, where bit 0 → digit 1 and bit 1 → digit 7.
 *
 *  Example:  n = 1,2 → len = 1, indices 0,1  → 1 , 7
 *           n = 3,4,5,6 → len = 2, indices 0,1,2,3 →
 *                           11, 17, 71, 77
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute the length of the decimal representation.
     * len = floor(log2(n+1))  (since numbers of length len start at n = 2^len - 1).
     */
    uint64_t x = n + 1ULL;           // safe for n < UINT64_MAX
    int len = (x == 0) ? 0 : (64 - __builtin_clzll(x) - 1);
    if (len <= 0) len = 1;           // only needed for the special case n = 0 (not a valid index)

    /* Index of the number within its length group (0‑based). */
    uint64_t idx = n - ((1ULL << len) - 1ULL);

    /* Build the decimal number by scanning bits from most significant to least. */
    seq_t val = 0;
    for (int i = 0; i < len; ++i) {
        int bit = (int)((idx >> (len - 1 - i)) & 1ULL);
        seq_t digit = bit ? 7 : 1;
        val = val * 10 + digit;
    }
    return val;
}
