/*
 * A375109  Number of distinct products i*j with 1 <= i, j <= n which are not
 *          the sum of two numbers between 1 and n.
 * https://oeis.org/A375109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS formula: a(n) = 1 + #{ distinct i·j  | 1≤i,j≤n , i·j > 2n }
 *  This is equivalent to a(n)=A373716(n)+A108954(n).  */

/*  We count all distinct products i*j that are larger than 2n.
 *  The product 1 (i=j=1) is always counted because 1 is never a
 *  sum of two numbers between 1 and n.  */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* bitset helpers ---------------------------------------------------- */
static inline void set_bit(uint64_t *bits, size_t pos) {
    bits[pos >> 6] |= 1ULL << (pos & 63);
}
static inline int test_bit(const uint64_t *bits, size_t pos) {
    return (bits[pos >> 6] >> (pos & 63)) & 1;
}

/* ----------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n < 1) return 0;                      /* offset starts at 1 */

    idx_t two_n   = 2 * n;
    idx_t max_prod = n * n;                    /* n≤10⁴, so ≤10⁸  */

    size_t words = (size_t)(max_prod >> 6) + 1;   /* one bit per value 0..max_prod */
    uint64_t *bits = (uint64_t *)calloc(words, sizeof(uint64_t));
    if (!bits) return 0;                     /* out‑of‑memory → undefined */

    seq_t count = 0;                          /* distinct products >2n */

    for (idx_t i = 1; i <= n; ++i) {
        idx_t j_start = two_n / i + 1;        /* first j with i*j > 2n */
        if (j_start > n) continue;
        for (idx_t j = j_start; j <= n; ++j) {
            idx_t prod = i * j;               /* prod ≤ n² ≤ 10⁸ */
            size_t pos = (size_t)prod;
            if (!test_bit(bits, pos)) {
                set_bit(bits, pos);
                ++count;
            }
        }
    }

    free(bits);
    return count + 1;                        /* add the product 1·1 */
}
