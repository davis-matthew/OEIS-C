/*
 * A274869  Number of set partitions of [n] into exactly five blocks such that
 *          all odd elements are in blocks with an odd index and all even
 *          elements are in blocks with an even index.
 * https://oeis.org/A274869
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) follows the linear recurrence a(n) = 2*a(n-1) + 9*a(n-2) -
 *            18*a(n-3) -20*a(n-4) +40*a(n-5) +12*a(n-6) -24*a(n-7) with
 *            initial values a(0)=a(1)=a(2)=a(3)=a(4)=0, a(5)=1
 * Range:     u8 n <= 10, u16 n <= 15, u32 n <= 28, u64 n <= 52, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This recurrence is obtained from the rational generating function
   x^5/((x-1)*(2*x-1)*(x+1)*(6*x^2-1)*(2*x^2-1)). */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    /* If called with n < 5, the value is undefined; return 0 by safety. */
    if (n < 5) return 0;

    /* Static storage for memoized sequence values. */
    static seq_t *seq = NULL;
    static idx_t cap = 0;
    static idx_t computed_up_to = 5;  /* last index computed (at least 5). */

    /* Expand storage if necessary. */
    if (cap <= n) {
        idx_t new_cap = n + 1;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!seq) exit(1);               /* allocation failure */
        if (cap == 0) {                  /* first allocation */
            memset(seq, 0, new_cap * sizeof(seq_t));
            seq[5] = (seq_t)1;
        } else if (new_cap > cap) {     /* zero newly allocated part */
            memset(seq + cap, 0, (new_cap - cap) * sizeof(seq_t));
        }
        cap = new_cap;
    }

    /* Compute sequence values up to n if not already done. */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        seq_t sum = 0;
        sum += (seq_t)2 * seq[i - 1];
        sum += (seq_t)9 * seq[i - 2];
        sum -= (seq_t)18 * seq[i - 3];
        sum -= (seq_t)20 * seq[i - 4];
        sum += (seq_t)40 * seq[i - 5];
        sum += (seq_t)12 * seq[i - 6];
        if (i >= 7) sum -= (seq_t)24 * seq[i - 7];
        seq[i] = sum;
    }
    computed_up_to = n > computed_up_to ? n : computed_up_to;

    return seq[n];
}
