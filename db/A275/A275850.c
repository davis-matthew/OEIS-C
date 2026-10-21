/*
 * A275850  Number of digits in range [1..A084558(n)] that do not occur in
 *          factorial base representation of n: a(n) = A084558(n) -
 *          A275806(n).
 * https://oeis.org/A275850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A084558(n) - A275806(n) * Here A084558(n) is the number
 *            of digits (length) of the factorial-base * representation of n,
 *            and A275806(n) is the number of distinct non‑zero * digits used
 *            in that representation. Thus a(n) equals the count of * integers
 *            in [1..len] not appearing among the digit...
 * Range:     u8 n <= 9902, u16 n <= 9902, u32 n <= 9902, u64 n <= 9902,
 *            u128 n <= 9902, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t temp = n;
    int len = 0;
    int distinct = 0;
    bool seen[21] = {false};      /* enough for idx_t up to 2^64-1 (max 20 digits) */

    int i = 1;
    while (temp > 0) {
        int digit = temp % (i + 1);   /* least significant digit of current position */
        if (digit != 0 && !seen[digit]) {
            seen[digit] = true;
            distinct++;
        }
        temp /= (i + 1);
        len++;
        i++;
    }
    return (seq_t)(len - distinct);
}
