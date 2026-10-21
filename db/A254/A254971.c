/*
 * A254971  Number of (n+2) X (1+2) 0..1 arrays with no 3 x 3 subblock
 *          diagonal sum 0 or 1 and no antidiagonal sum 0 or 1 and no row sum
 *          0 and no column sum 0.
 * https://oeis.org/A254971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) + 3*a(n-3) + 5*a(n-4) - a(n-5) -
 *            6*a(n-6) - 7*a(n-7) + 2*a(n-9) - a(n-10) for n>11 Initial
 *            values: a(1)=121, a(2)=310, a(3)=1008, a(4)=3241, a(5)=9961,
 *            a(6)=30529, a(7)=94351, a(8)=292130, a(9)=902991, a(10)=2789674,
 *            a(11)=8620990, a(12)=26645405
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 16, u64 n <= 36, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *seq_cache = NULL;
static idx_t     seq_cache_len = 0;   /* highest index cached (1‑based) */
static idx_t     seq_cache_cap = 0;   /* size of allocated cache array */

static void seq_cache_ensure(idx_t n)
{
    /* initialize cache on first use */
    if (seq_cache == NULL) {
        seq_cache_cap = 13;                 /* cache indices 0..12, ignore 0 */
        seq_cache = (seq_t *)malloc(seq_cache_cap * sizeof(seq_t));
        if (!seq_cache) return;             /* allocation failure – unlikely */
        memset(seq_cache, 0, seq_cache_cap * sizeof(seq_t));
        seq_cache[1]  = (seq_t)121;
        seq_cache[2]  = (seq_t)310;
        seq_cache[3]  = (seq_t)1008;
        seq_cache[4]  = (seq_t)3241;
        seq_cache[5]  = (seq_t)9961;
        seq_cache[6]  = (seq_t)30529;
        seq_cache[7]  = (seq_t)94351;
        seq_cache[8]  = (seq_t)292130;
        seq_cache[9]  = (seq_t)902991;
        seq_cache[10] = (seq_t)2789674;
        seq_cache[11] = (seq_t)8620990;
        seq_cache[12] = (seq_t)26645405;
        seq_cache_len = 12;
    }

    /* extend allocation if necessary */
    if (n + 1 > seq_cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(seq_cache,
                                            new_cap * sizeof(seq_t));
        if (new_cache == NULL) return;   /* keep old cache on failure */
        seq_cache = new_cache;
        seq_cache_cap = new_cap;
        /* new slots already zeroed by realloc for most implementations;
           but zero them to be safe. */
        memset(seq_cache + seq_cache_len + 1,
               0, (new_cap - seq_cache_len - 1) * sizeof(seq_t));
    }
}

seq_t a(idx_t n)
{
    /* Ensure cache contains values up to n */
    seq_cache_ensure(n);

    if (n <= seq_cache_len) return seq_cache[n];

    /* Compute sequence values from the cached upper bound up to n */
    for (idx_t i = seq_cache_len + 1; i <= n; ++i) {
        /* Using unsigned arithmetic is safe because the true result
           is always non‑negative and the intermediate sums are
           never larger than the final value within the tested range. */
        seq_t val = 0;

        /* 2*a(i-1) + 2*a(i-2) + 3*a(i-3) + 5*a(i-4) + 2*a(i-9) */
        val += 2 * seq_cache[i - 1];
        val += 2 * seq_cache[i - 2];
        val += 3 * seq_cache[i - 3];
        val += 5 * seq_cache[i - 4];
        val += 2 * seq_cache[i - 9];

        /* subtract the remaining terms */
        val -= seq_cache[i - 5];
        val -= 6 * seq_cache[i - 6];
        val -= 7 * seq_cache[i - 7];
        val -= seq_cache[i - 10];

        seq_cache[i] = val;
    }

    seq_cache_len = n;
    return seq_cache[n];
}
