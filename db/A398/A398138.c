/*
 * A398138  Sum of the remainders k mod n over the distinct entries k of the n
 *          X n multiplication table.
 * https://oeis.org/A398138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = sum of k mod n over distinct products k = i*j
   of the n×n multiplication table (OEIS A398138). */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;             /* out of range, but safe */

    idx_t max_prod = n * n;                  /* product never exceeds n²   */
    size_t bitset_bytes = (size_t)((max_prod >> 3) + 1); /* 8 bits per byte */

    /* calloc may return void* in C++ – cast explicitly. */
    unsigned char *seen = (unsigned char *)calloc(bitset_bytes, 1);
    if (!seen) return (seq_t)0;              /* allocation failure → 0     */

    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; ++j) {     /* j >= i to avoid duplicates  */
            idx_t prod = i * j;              /* product ≤ n²                 */
            idx_t byte = prod >> 3;          /* prod / 8                     */
            unsigned char mask = 1U << (prod & 7);
            if (!(seen[byte] & mask)) {
                seen[byte] |= mask;          /* mark product as seen         */
                sum += (seq_t)(prod % n);    /* accumulate remainder        */
            }
        }
    }

    free(seen);
    return sum;
}
