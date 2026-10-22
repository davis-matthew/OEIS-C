/*
 * A378360  a(1) = 0, a(n) = Sum_{digits d in a(n-1)} (d+[d==0])*c(d,n-1)
 *          where c(d,k) = number of digits d in a(1..a(k)), with Iverson
 *          bracket.
 * https://oeis.org/A378360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{distinct digits d in a(n-1)} (d+[d==0]) * c(d,n-1),
 *            * where c(d,k) is the total number of occurrences of digit d in
 *            the terms a(1)…a(k)
 * Range:     u8 n <= 94, u16 n <= 4960, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* static state for memoisation */
    static seq_t *seq = NULL;   /* array of sequence values, 1-indexed */
    static size_t cap = 0;      /* allocated capacity */
    static idx_t len = 0;       /* highest computed index */
    static uint64_t count[10] = {0};     /* cumulative digit counts */

    if (n < 1) return 0;   /* undefined for indices < 1, return 0 */

    /* initialise on first call */
    if (seq == NULL) {
        cap = 128;
        seq = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!seq) return 0; /* allocation failure */
        seq[1] = 0;          /* a(1) */
        len = 1;
        count[0] = 1;        /* digit 0 occurs once in a(1) */
    }

    /* grow storage if required */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while ((size_t)n >= new_cap) new_cap *= 2;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t m = len + 1; m <= n; ++m) {
        seq_t prev = seq[m - 1];
        seq_t sum = 0;
        int seen[10] = {0};

        /* process distinct digits of prev */
        if (prev == 0) {
            seen[0] = 1;
            sum += (seq_t)(1) * (seq_t)count[0];  /* (0+1) * count[0] */
        } else {
            seq_t temp = prev;
            while (temp > 0) {
                int d = (int)(temp % 10);
                if (!seen[d]) {
                    seen[d] = 1;
                    sum += (seq_t)(d + (d == 0)) * (seq_t)count[d];
                }
                temp /= 10;
            }
        }

        seq[m] = sum;

        /* update cumulative counts with digits of the new term (allow repetitions) */
        if (sum == 0) {
            count[0] += 1;
        } else {
            seq_t tmp = sum;
            while (tmp > 0) {
                int d = (int)(tmp % 10);
                ++count[d];
                tmp /= 10;
            }
        }
    }

    len = n;
    return seq[n];
}
