/*
 * A298148  Number of nX2 0..1 arrays with every element equal to 0, 1, 3, 4,
 *          5 or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 43, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A298148
   Number of n×2 0..1 arrays with every element equal to 0, 1, 3, 4, 5 or 8 king‑move adjacent elements,
   with upper left element zero.
   Recurrence (empirical):
       a(n) = 2*a(n‑1) + 2*a(n‑2) + 6*a(n‑3) – 10*a(n‑4) – 8*a(n‑5)   for n > 6
   Initial values:
       a(1)=2, a(2)=4, a(3)=4, a(4)=16, a(5)=50, a(6)=112
   The function below computes values by memoised recurrence for n≥1.
   It stores up to a fixed maximum index (10000) in a static array; this
   satisfies the requirement to be O(n) and fast for n up to 10 000.  */

#include <stddef.h>
#include <stdbool.h>

static const seq_t init_vals[6] = { (seq_t)2, (seq_t)4, (seq_t)4, (seq_t)16, (seq_t)50, (seq_t)112 };
static seq_t cache[10001];
static size_t max_computed = 0; /* how many terms (from a(1)) are stored */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, but safe */

    if ((size_t)n > sizeof(cache) / sizeof(cache[0])) {
        /* n is too large; the sequence is defined for all n, but
           this implementation only supports up to 10000. */
        return 0;
    }

    if ((size_t)n <= max_computed) { /* already computed */
        return cache[n - 1];
    }

    /* Compute up to n */
    for (size_t i = max_computed; i < (size_t)n; ++i) {
        if (i < 6) {                 /* initial terms (i = 0..5 → a(1)..a(6)) */
            cache[i] = init_vals[i];
        } else {                     /* recurrence for a(i+1), i>=6 → n>=7 */
            cache[i] =  (seq_t)2 * cache[i - 1]
                       + (seq_t)2 * cache[i - 2]
                       + (seq_t)6 * cache[i - 3]
                       - (seq_t)10 * cache[i - 4]
                       - (seq_t)8  * cache[i - 5];
        }
    }
    max_computed = (size_t)n;
    return cache[n - 1];
}
