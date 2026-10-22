/*
 * A300925  Number of nX3 0..1 arrays with every element equal to 1, 2, 3, 4
 *          or 5 horizontally, vertically or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A300925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   recurrence for n > 10: a(n) = 6*a(n-1) + 6*a(n-2) - 3*a(n-3) -
 *            9*a(n-4) - 6*a(n-5) + 3*a(n-6) + 6*a(n-7) + 2*a(n-8) initial
 *            values: a(1)=1 a(2)=11 a(3)=75 a(4)=508 a(5)=3447 a(6)=23404
 *            a(7)=158855 a(8)=1078231 a(9)=7318522 a(10)=49674630
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *seq_cache = NULL;         /* array of cached values, indexed by n */
static size_t  seq_cap   = 0;           /* current capacity of seq_cache */
static idx_t   seq_max   = 0;           /* largest n that has been computed */

/* Ensure that seq_cache has capacity for indices up to 'need' (inclusive). */
static void ensure_capacity(size_t need)
{
    if (seq_cap >= need + 1)               /* +1 so that index 'need' is valid */
        return;

    size_t new_cap = seq_cap ? seq_cap : 64;
    while (new_cap <= need)
        new_cap *= 2;

    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Compute a(n) for any integer n >= 1. Cache results for future calls. */
seq_t a(idx_t n)
{
    if (n < 1)            /* out of range; undefined */
        return 0;

    /* prepare cache up to n */
    ensure_capacity((size_t)n);

    /* compute missing entries */
    if (n > seq_max) {
        for (idx_t i = seq_max + 1; i <= n; ++i) {
            seq_t val = 0;
            /* initial values */
            switch (i) {
                case 1:  val = (seq_t)1;          break;
                case 2:  val = (seq_t)11;         break;
                case 3:  val = (seq_t)75;          break;
                case 4:  val = (seq_t)508;         break;
                case 5:  val = (seq_t)3447;        break;
                case 6:  val = (seq_t)23404;       break;
                case 7:  val = (seq_t)158855;      break;
                case 8:  val = (seq_t)1078231;     break;
                case 9:  val = (seq_t)7318522;     break;
                case 10: val = (seq_t)49674630;    break;
                default:
                    /* recurrence for i > 10 */
                    val =  6 * seq_cache[(size_t)(i-1)]
                        + 6 * seq_cache[(size_t)(i-2)]
                        - 3 * seq_cache[(size_t)(i-3)]
                        - 9 * seq_cache[(size_t)(i-4)]
                        - 6 * seq_cache[(size_t)(i-5)]
                        + 3 * seq_cache[(size_t)(i-6)]
                        + 6 * seq_cache[(size_t)(i-7)]
                        + 2 * seq_cache[(size_t)(i-8)];
                    break;
            }
            seq_cache[(size_t)i] = val;
        }
        seq_max = n;
    }

    return seq_cache[(size_t)n];
}
