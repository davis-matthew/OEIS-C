/*
 * A247372  List of b, where b is positive integer solutions of 1/a + 1/b =
 *          1/c and a>b>c and gcd(a,b,c)=1, sorted by ascending a, then b.
 * https://oeis.org/A247372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A247372: the b‑values obtained from the parametrization
   a = m(m+n), b = n(m+n), c = m*n with coprime m>n.  
   The pairs are sorted by ascending a and then ascending b. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

typedef struct {
    seq_t a;
    seq_t b;
} pair_t;

static pair_t *pair_arr = NULL;
static size_t  pair_cnt = 0;
static int     init_done = 0;

static int pair_cmp(const void *p1, const void *p2)
{
    const pair_t *e1 = (const pair_t *)p1;
    const pair_t *e2 = (const pair_t *)p2;
    if (e1->a < e2->a) return -1;
    if (e1->a > e2->a) return 1;
    if (e1->b < e2->b) return -1;
    if (e1->b > e2->b) return 1;
    return 0;
}

static void init_pairs(void)
{
    if (init_done) return;
    const size_t M = 400;                     /* enough for n≈10 000 */
    size_t est = (size_t)(0.31 * M * M);      /* rough estimate */
    pair_arr = (pair_t *)malloc(est * sizeof(pair_t));
    if (!pair_arr) exit(EXIT_FAILURE);
    pair_cnt = 0;

    for (size_t m = 2; m <= M; ++m) {
        seq_t m_seq = (seq_t)m;
        for (size_t n = 1; n < m; ++n) {
            if (gcd(m_seq, (seq_t)n) == 1) {
                seq_t sum = m_seq + (seq_t)n;
                pair_arr[pair_cnt].a = m_seq * sum;
                pair_arr[pair_cnt].b = (seq_t)n * sum;
                ++pair_cnt;
            }
        }
    }
    pair_arr = (pair_t *)realloc(pair_arr, pair_cnt * sizeof(pair_t));
    qsort(pair_arr, pair_cnt, sizeof(pair_t), pair_cmp);
    init_done = 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined; safe fallback */
    init_pairs();
    if (n > pair_cnt) return (seq_t)0;   /* out of bounds */
    size_t idx = (size_t)(n - 1);
    return pair_arr[idx].b;
}
