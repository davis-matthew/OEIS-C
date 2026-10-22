/*
 * A308199  The tribonacci representation of a(n) is obtained by appending 0,0
 *          to the tribonacci representation of n (cf. A278038).
 * https://oeis.org/A308199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{k=0}^{n-1} d(k), * where d is the infinite tribonacci
 *            word with * alphabet letters mapped to digits {4,3,2}
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>


/*  a(0)=0, a(1)=4, a(2)=7, a(3)=11, a(4)=13, a(5)=17, ...  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* work in terms of size_t for lengths and allocations  */
    size_t target = (size_t)n;
    size_t alloc = target * 2 + 10;   /* enough for the next word */

    char *cur  = (char *)malloc(alloc);
    char *nxt  = (char *)malloc(alloc);
    if (!cur || !nxt) {
        free(cur);
        free(nxt);
        return 0;                     /* unlikely for n ≤ 10000 */
    }

    cur[0] = 4;                /* 'a' → 4  */
    size_t len = 1;

    /* grow the word until it contains at least 'n' symbols   */
    while (len < target) {
        size_t next_len = 0;
        for (size_t i = 0; i < len; ++i) {
            char v = cur[i];
            if (v == 4) {                 /* 'a' → 4,3 */
                nxt[next_len++] = 4;
                nxt[next_len++] = 3;
            } else if (v == 3) {           /* 'b' → 4,2 */
                nxt[next_len++] = 4;
                nxt[next_len++] = 2;
            } else {                       /* 'c' → 4   */
                nxt[next_len++] = 4;
            }
        }
        /* swap roles of the two buffers */
        char *tmp = cur;
        cur = nxt;
        nxt = tmp;
        len = next_len;
    }

    seq_t sum = 0;
    for (size_t i = 0; i < target; ++i)
        sum += (seq_t)cur[i];

    free(cur);
    free(nxt);
    return sum;
}
