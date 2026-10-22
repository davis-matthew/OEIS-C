/*
 * A333716  a(0)=1; for n>0, a(n) is the greatest common divisor (GCD) of n
 *          and the sum of the previous terms back to the last GCD term, if
 *          the GCD is not already in the sequence; otherwise a(n) = a(n-1) +
 *          n.
 * https://oeis.org/A333716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS sequence A333716
   a(0) = 1
   for n > 0:
     let S = sum of all terms from the last term that was a GCD up to a(n-1)
     g = gcd(S, n)
     if g has already appeared in the sequence:
         a(n) = a(n-1) + n
         S = S + a(n)
     else:
         a(n) = g        // new GCD term
         S = g           // reset sum to this GCD
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t *seq = NULL;        // dynamic array of computed terms
static size_t  seq_cap = 0;      // allocated capacity of seq
static size_t  seq_len = 0;      // number of terms currently stored
static seq_t  running_sum = 0;   // sum of terms since last GCD term

/* ensure capacity for at least new_cap elements */
static void ensure_cap(size_t new_cap) {
    if (new_cap <= seq_cap) return;
    size_t new_cap2 = (seq_cap == 0) ? 16 : seq_cap << 1;
    while (new_cap2 < new_cap) new_cap2 <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap2 * sizeof(seq_t));
    seq = tmp;
    seq_cap = new_cap2;
}

/* initialize the sequence storage with a(0) = 1 */
static void init_sequence(void) {
    if (seq_len == 0) {
        ensure_cap(1);
        seq[0] = (seq_t)1;
        seq_len = 1;
        running_sum = seq[0];  // sum of terms since last GCD (a(0) is a GCD term)
    }
}

/* check whether value 'v' has appeared in the sequence up to current length */
static bool seen_before(seq_t v) {
    for (size_t i = 0; i < seq_len; ++i)
        if (seq[i] == v) return true;
    return false;
}

/* compute a(n) for any n >= 0, memoizing partial results */
seq_t a(idx_t n) {
    init_sequence();                       /* ensure a(0) is present */
    size_t target = (size_t)n;

    if (target < seq_len)          // already computed
        return seq[target];

    /* compute missing terms up to target */
    for (size_t i = seq_len; i <= target; ++i) {
        seq_t g = gcd(running_sum, (seq_t)i);

        bool exists = seen_before(g);

        seq_t val;
        if (!exists) {                         /* new GCD term */
            val = g;
            running_sum = val;                 /* reset sum */
        } else {                               /* not a GCD term */
            val = seq[seq_len - 1] + (seq_t)i;  /* last term + n */
            running_sum += val;                /* extend current sum */
        }

        ensure_cap(seq_len + 1);
        seq[seq_len++] = val;
    }

    return seq[target];
}
