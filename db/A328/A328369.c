/*
 * A328369  Numbers without repeated parts in their partitions into
 *          consecutive parts.
 * https://oeis.org/A328369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth positive integer m such that no part appears in more
 *            than one partition of m into consecutive positive integers
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Return true if m has no repeated part across its consecutive-part
   partitions. */
static bool good(seq_t m) {
    /* m will fit into idx_t for the range of n we support. */
    idx_t m_val = (idx_t)m;
    size_t seen_size = (size_t)(m_val + 1);
    unsigned char *seen = (unsigned char*)malloc(seen_size);
    if (!seen) return false;      /* out of memory – treat as bad */
    memset(seen, 0, seen_size);

    /* Enumerate all lengths k of consecutive-part partitions. */
    for (idx_t k = 1; ; ++k) {
        /* minimal sum for k parts a,a+1,...,a+k-1 is k*(k+1)/2. */
        seq_t min_sum = (seq_t)k * (k + 1) / 2;
        if (min_sum > m) break;

        /* Compute numerator = m - k*(k-1)/2. */
        seq_t num = m - (seq_t)k * (k - 1) / 2;
        if (num % k != 0) continue;
        seq_t a = num / k;
        if (a < 1) continue;

        /* Each part a + i must be unique among all partitions. */
        for (idx_t i = 0; i < k; ++i) {
            seq_t part = a + i;
            idx_t p = (idx_t)part;      /* part fits into idx_t here. */
            if (p >= seen_size) {      /* safety check */
                free(seen);
                return false;
            }
            if (seen[p]) {
                free(seen);
                return false;          /* repeat found */
            }
            seen[p] = 1;                /* mark part as seen */
        }
    }

    free(seen);
    return true;
}

/* Public interface: return nth term of the sequence. */
seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t m = 1;
    while (true) {
        if (good(m)) {
            ++count;
            if (count == n) return m;
        }
        ++m;
    }
}
