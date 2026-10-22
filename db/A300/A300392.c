/*
 * A300392  a(1)=1, a(n) = max(sum0,sum1) mod n, where sum0 is the sum of
 *          previous terms with even indices, sum1 with odd indices.
 * https://oeis.org/A300392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1 For n>=2: Let sum0 be the sum of a(i) with even i < n,
 *            let sum1 be the sum of a(i) with odd i < n. Then a(n) =
 *            max(sum0,sum1) mod n
 * Range:     u8 n <= 54, u16 n <= 941, u32 n <= 9922, u64 n <= 9922,
 *            u128 n <= 9922, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

static seq_t *seq = NULL;           /* storage for computed terms */
static size_t seq_cap = 0;          /* allocated capacity of seq */
static idx_t max_n = 0;             /* number of terms currently stored */
static seq_t even_sum = 0;          /* sum of a(i) for even i <= max_n */
static seq_t odd_sum  = 0;          /* sum of a(i) for odd  i <= max_n */

/* Compute a(n) for any n >= 1, with memoisation. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Handle n==1 specially, initialise if necessary */
    if (n == 1) {
        if (max_n == 0) {
            /* allocate minimal space */
            seq_cap = 1;
            seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
            if (!seq) exit(1);
            seq[0] = (seq_t)1;
            odd_sum = seq[0];
            even_sum = (seq_t)0;
            max_n = 1;
        }
        return seq[0];
    }

    /* If we have not yet started, initialise a(1) */
    if (max_n == 0) {
        seq_cap = 1;
        seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        if (!seq) exit(1);
        seq[0] = (seq_t)1;
        odd_sum = seq[0];
        even_sum = (seq_t)0;
        max_n = 1;
    }

    /* Extend the computed sequence up to n if needed */
    if (n > max_n) {
        /* Ensure enough capacity */
        size_t new_cap = seq_cap;
        while (new_cap < (size_t)n) new_cap = new_cap ? new_cap * 2 : 1;
        if (new_cap > seq_cap) {
            seq_t *tmp = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            seq = tmp;
            seq_cap = new_cap;
        }

        /* Compute terms from max_n+1 up to n */
        for (idx_t cur = max_n + 1; cur <= n; ++cur) {
            seq_t best = even_sum > odd_sum ? even_sum : odd_sum;
            seq_t val  = best % (seq_t)cur;
            seq[cur - 1] = val;

            if (cur & 1U)   /* odd index */
                odd_sum += val;
            else            /* even index */
                even_sum += val;

            max_n = cur;
        }
    }

    return seq[n - 1];
}
