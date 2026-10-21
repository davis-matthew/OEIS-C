/*
 * A280696  Largest Lucas proper divisor of n, a(1) = a(2) = 1.
 * https://oeis.org/A280696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = greatest Lucas number that divides n and is less than n;
 *            a(1) = a(2) = 1
 * Range:     u8 n <= 446, u16 n <= 9767, u32 n <= 9767, u64 n <= 9767,
 *            u128 n <= 9767, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)1;

    /* Cached Lucas numbers generated in the order L0 = 2, L1 = 1, L2 = 3, ... */
    static seq_t *lucas = NULL;
    static size_t lucas_len = 0, lucas_cap = 0;

    if (lucas == NULL) {
        lucas_cap = 8;
        lucas = (seq_t *)malloc(lucas_cap * sizeof(seq_t));
        if (!lucas)
            return (seq_t)0;                     /* allocation failure */
        lucas[0] = (seq_t)2;                     /* L0 */
        lucas[1] = (seq_t)1;                     /* L1 */
        lucas_len = 2;
    }

    seq_t n_seq = (seq_t)n;

    /* Grow the list until the last generated Lucas number exceeds n_seq. */
    while (lucas_len >= 2 && lucas[lucas_len - 1] <= n_seq) {
        seq_t next = lucas[lucas_len - 1] + lucas[lucas_len - 2];

        if (lucas_len == lucas_cap) {
            lucas_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(lucas, lucas_cap * sizeof(seq_t));
            if (!tmp)
                return (seq_t)0;                 /* allocation failure */
            lucas = tmp;
        }
        lucas[lucas_len++] = next;
    }

    /* Find the largest Lucas divisor that is strictly less than n. */
    seq_t best = (seq_t)1; /* 1 always divides n */
    for (size_t i = 0; i < lucas_len; ++i) {
        seq_t L = lucas[i];
        if (L < n_seq && n_seq % L == 0 && L > best)
            best = L;
    }
    return best;
}
