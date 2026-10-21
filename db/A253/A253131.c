/*
 * A253131  Number of length 3+2 0..n arrays with the sum of medians of
 *          adjacent triples multiplied by some arrangement of +-1 equal to
 *          zero.
 * https://oeis.org/A253131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - a(n-2) -5*a(n-3) +5*a(n-4) + a(n-5) -3*a(n-6)
 *            + a(n-7), a(1)=16, a(2)=90, a(3)=281, a(4)=673, a(5)=1356,
 *            a(6)=2452, a(7)=4083
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 121, u64 n <= 121,
 *            u128 n <= 121, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;   /* cache[0] = a(1) */
static idx_t  cached = 0;     /* number of terms cached (>=7 when initialized) */

/* Initialize cache with first 7 terms */
static void ensure_initial(void)
{
    if (cached > 0) return;
    cache = (seq_t *)malloc(8 * sizeof(seq_t));
    if (!cache) return;      /* allocation failure -- caller will crash on use */
    cache[0] = (seq_t)16;
    cache[1] = (seq_t)90;
    cache[2] = (seq_t)281;
    cache[3] = (seq_t)673;
    cache[4] = (seq_t)1356;
    cache[5] = (seq_t)2452;
    cache[6] = (seq_t)4083;
    cached = 7;
}

/* Computes sequence a(n) for n >= 1 */
seq_t a(idx_t n)
{
    if (n < 1) return 0;        /* offset is 1; behaviour for n<1 undefined */

    ensure_initial();

    /* Return base values directly if requested */
    if (n <= 7) return cache[n-1];

    /* Grow cache if necessary */
    if (cached < n) {
        seq_t *new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!new_cache) return 0;  /* out of memory */
        cache = new_cache;
        for (idx_t i = cached + 1; i <= n; ++i) {
            /* recurrence: clean unsigned arithmetic using successive subtraction */
            seq_t term1 = 3 * cache[i-2];        /* 3*a(i-1) */
            seq_t term4 = 5 * cache[i-5];        /* 5*a(i-4) */
            seq_t term5 =   cache[i-6];          /*   a(i-5) */
            seq_t term7 =   cache[i-8];          /*   a(i-7) */
            seq_t sum_pos = term1 + term4 + term5 + term7;

            seq_t term2 =   cache[i-3];          /*   a(i-2) */
            seq_t term3 = 5 * cache[i-4];        /* 5*a(i-3) */
            seq_t term6 = 3 * cache[i-7];        /* 3*a(i-6) */
            seq_t result = sum_pos;
            result -= term2;                     /* subtract a(i-2) */
            result -= term3;                     /* subtract 5*a(i-3) */
            result -= term6;                     /* subtract 3*a(i-6) */

            cache[i-1] = result;
        }
        cached = n;
    }
    return cache[n-1];
}
