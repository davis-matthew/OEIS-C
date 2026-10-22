/*
 * A319894  Number of ordered pairs (i,j) with 0 < i < j < prime(n)/2 such
 *          that R(i^4,prime(n)) > R(j^4,prime(n)), where R(k,p) (with p an
 *          odd prime) denotes the unique integer r among 0,...,(p-1)/2 with k
 *          congruent to r or -r modulo p.
 * https://oeis.org/A319894
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered pairs (i,j) with 0 < i < j < p/2 such
 *            that R(i^4,p) > R(j^4,p), where p = prime(n) and R(k,p) is the
 *            unique r in [0,(p-1)/2] with k ≡ ±r (mod p)
 * Range:     u8 n <= 20, u16 n <= 173, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   R_of_i4, bit_query, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/get_prime.h"    /* get_prime(idx_t) */
#include "../utils/R_of_i4.h"
#include "../utils/bit_query.h"

/* Fenwick tree (Binary Indexed Tree) for counting frequencies. */
static void bit_update(uint32_t *bit, uint32_t idx, uint32_t val, uint32_t size)
{
    /* 0‑based idx, tree is 1‑based */
    idx++;                           /* shift to 1‑based */
    for (; idx <= size; idx += idx & -idx)
        bit[idx] += val;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                     /* undefined below offset */

    /* Retrieve the n‑th prime (1‑based). */
    uint32_t p = (uint32_t)get_prime(n);
    uint32_t half = (p - 1) / 2;       /* values range 0..half */

    /* Fenwick tree of size half+1 (values 0..half). */
    uint32_t size = half + 1;
    uint32_t *bit = (uint32_t *)calloc((size_t)size + 1, sizeof(uint32_t));
    if (!bit)
        return 0;                     /* unlikely but safe */

    seq_t total = 0;

    for (uint32_t i = 1; i <= half; ++i) {
        uint32_t r = R_of_i4(i, p, half);       /* 0 .. half */
        uint32_t cnt_le = bit_query(bit, r);    /* elements ≤ r seen so far */
        uint32_t prev = i - 1;
        uint32_t greater = prev - cnt_le;       /* elements > r seen so far */
        total += (seq_t)greater;
        bit_update(bit, r, 1, size);
    }

    free(bit);
    return total;
}
