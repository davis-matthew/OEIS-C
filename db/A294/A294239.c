/*
 * A294239  Sum of the parts in the partitions of n into two parts with larger
 *          part nonsquarefree.
 * https://oeis.org/A294239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * \sum_{i=1..⌊n/2⌋} (1 - mu(n-i)^2)
 * Range:     u8 n <= 36, u16 n <= 575, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static idx_t mobmax = 0;
static int8_t *mob = NULL;

/* Build the Möbius function up to 'limit'.  Overwrites any past table. */
static void extend_mobius(idx_t limit)
{
    if (mobmax >= limit) return;
    /* free old table if present */
    if (mob) free(mob);
    mob = (int8_t*)malloc((limit + 1) * sizeof(int8_t));
    int *pr = (int*)malloc((limit + 1) * sizeof(int));
    int *mind = (int*)malloc((limit + 1) * sizeof(int));
    if (!mob || !pr || !mind) exit(1);          /* unrecoverable error */

    for (idx_t i = 0; i <= limit; ++i) {
        mob[i] = 0;
        mind[i] = 0;
    }
    mob[1] = 1;
    int pc = 0;                                 /* prime count */

    for (idx_t i = 2; i <= limit; ++i) {
        if (!mind[i]) {                         /* i is prime */
            mind[i] = (int)i;
            mob[i] = -1;
            pr[pc++] = (int)i;
        }
        for (int j = 0; j < pc && pr[j] <= mind[i] && (idx_t)i * pr[j] <= limit; ++j) {
            idx_t p = (idx_t)pr[j];
            mind[i * p] = (int)p;
            if (p == mind[i])
                mob[i * p] = 0;
            else
                mob[i * p] = -mob[i];
        }
    }
    free(pr);
    free(mind);
    mobmax = limit;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of range, but safe */
    extend_mobius(n * 2);           /* ensure mu is defined up to n-1 */
    idx_t up = n / 2;
    idx_t cnt = 0;
    for (idx_t i = 1; i <= up; ++i) {
        idx_t m = n - i;
        if (mob[m] == 0) ++cnt;     /* not squarefree */
    }
    return (seq_t)n * (seq_t)cnt;
}
