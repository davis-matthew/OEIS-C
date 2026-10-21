/*
 * A260183  Expansion of f(x, x^2) * f(x^4, x^8) / f(-x^3, -x^6)^2 in powers
 *          of x where f(, ) is Ramanujan's general theta function.
 * https://oeis.org/A260183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 59, u32 n <= 179, u64 n <= 595,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = coefficient of x^n in
 *   ∏_{k≥1} (1 - x^k)^(-b_k)
 * where b_k = 1 if k mod 24 ∈ {1,3,4,5,6,7,9,11,13,15,17,18,19,21,23}
 *            0 otherwise.
 * Equivalently, a(n) is the number of partitions of n into parts whose
 * sizes are in the set above.
 *
 * This implementation recomputes the partition numbers for each call
 * in O(n^2) time and uses a static buffer that grows as needed.
 * It works for any seq_t and idx_t as specified.
 */

#include <stdlib.h>
#include <string.h>

/* period pattern of length 24 */
static const unsigned char period[24] = {
    1,0,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,0,1,0
};

static seq_t *buffer = NULL;   /* dynamic array for DP values */
static idx_t buffer_cap = 0;   /* number of elements allocated */

/* Return the n-th term of the sequence A260183. */
seq_t a(idx_t n)
{
    /* Grow the buffer if necessary */
    if (buffer_cap <= n) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(buffer, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;        /* allocation failure */
        buffer = tmp;
        buffer_cap = new_cap;
    }

    /* Initialize to zero and set dp[0] = 1 */
    memset(buffer, 0, (n + 1) * sizeof(seq_t));
    buffer[0] = (seq_t)1;

    /* Dynamic programming over allowed part sizes */
    for (idx_t k = 1; k <= n; ++k) {
        if (period[(k - 1) % 24] == 0) continue;   /* forbidden part */
        for (idx_t j = k; j <= n; ++j) {
            buffer[j] += buffer[j - k];
        }
    }

    return buffer[n];
}
