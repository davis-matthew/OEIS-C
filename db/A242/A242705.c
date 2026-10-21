/*
 * A242705  Lexicographically earliest sequence of distinct positive integers
 *          such that for any n>0 the number of ones in the binary
 *          representation of n divides a(n).
 * https://oeis.org/A242705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   lexicographically earliest permutation of the positive integers
 *            such that for every n>0 the popcount of n divides a(n). The
 *            sequence is constructed greedily: at each step choose the
 *            smallest unused multiple of popcount(n)
 * Range:     u8 n <= 177, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Helper: popcount for 64‑bit unsigned indices */
static inline int popcount_idx(idx_t x)
{
    return __builtin_popcountll((unsigned long long)x);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)      /* undefined for n < 1 – return 0 */
        return 0;

    /* Array of next unused multiple for each popcount (1..64) */
    seq_t next_mul[65];
    for (int i = 0; i < 65; ++i)
        next_mul[i] = 0;

    /* Boolean array to record which numbers have appeared.
       Grow dynamically as needed. */
    size_t used_size = 0;
    unsigned char *used = NULL;

    seq_t result = 0;

    for (idx_t idx = 1; idx <= n; ++idx) {
        int pc = popcount_idx(idx);
        seq_t cand = (next_mul[pc] == 0) ? (seq_t)pc : next_mul[pc];

        /* Find the smallest unused multiple of pc. */
        while (1) {
            idx_t ci = (idx_t)cand;
            if (ci >= used_size)          /* beyond known used range: unused */
                break;
            if (!used[ci])                /* unused inside known range */
                break;
            cand += pc;                    /* skip used value */
        }

        /* Register the chosen value as used, enlarging the array if necessary. */
        idx_t ci = (idx_t)cand;
        if (ci >= used_size) {
            size_t new_size = used_size ? used_size * 2 : 8;
            while (new_size <= ci) new_size <<= 1;
            unsigned char *tmp = (unsigned char *)realloc(used, new_size);
            if (!tmp) abort();            /* allocation failure */
            memset(tmp + used_size, 0, new_size - used_size);
            used = tmp;
            used_size = new_size;
        }
        used[ci] = 1;

        /* Update the next‑candidate pointer for this popcount. */
        seq_t nxt = cand + (seq_t)pc;
        while (1) {
            idx_t ni = (idx_t)nxt;
            if (ni >= used_size)
                break;                    /* not yet used */
            if (!used[ni])
                break;                    /* unused */
            nxt += (seq_t)pc;            /* skip used value */
        }
        next_mul[pc] = nxt;

        /* Store result for requested index. */
        if (idx == n)
            result = cand;
    }

    free(used);
    return result;
}
