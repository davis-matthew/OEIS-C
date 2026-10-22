/*
 * A351850  a(n) is the number of iterations of the computation of the A351849
 *          tag system when started from the word encoding n, or -1 if the
 *          number of iterations is infinite.
 * https://oeis.org/A351850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of iterations of the A351849 tag system *
 *            starting from the word encoding n. * a(1) = 0 * For n > 1: * if
 *            n even: a(n) = n + a(n/2) * if n odd : a(n) = n + 1 +
 *            a((3n+1)/2) * 1 is the offset
 * Range:     u8 n <= 14, u16 n <= 326, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define CACHE_SIZE 30001   /* sufficient for all intermediate values */

static seq_t cache[CACHE_SIZE];
static bool done[CACHE_SIZE];

static seq_t tag_collatz(idx_t n)
{
    if (n == 1)
        return 0;                   /* a(1) = 0 */

    if (n < CACHE_SIZE && done[n])
        return cache[n];            /* memoised result */

    seq_t res;
    if ((n & 1ull) == 0ull) {        /* even */
        seq_t next = tag_collatz(n / 2);
        res = (seq_t)n + next;      /* n + a(n/2) */
    } else {                        /* odd */
        seq_t next = tag_collatz((3 * n + 1) / 2);
        res = (seq_t)n + 1 + next;  /* n+1 + a((3n+1)/2) */
    }

    if (n < CACHE_SIZE) {
        cache[n] = res;
        done[n] = true;
    }
    return res;
}

seq_t a(idx_t n)
{
    return tag_collatz(n);
}
