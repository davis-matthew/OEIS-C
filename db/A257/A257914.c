/*
 * A257914  Pandigital numbers reordered so that the numbers
 *          A050278(n)/(2^k*5^m), where 2^k||A050278(n) and 5^m||A050278(n),
 *          appear in nondecreasing order.
 * https://oeis.org/A257914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A257914: 10‑digit pandigital numbers reordered by
   their reduced values after removing all factors of 2 and 5.
   The list is generated on the first call, sorted once,
   and then indexed for all subsequent calls. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Store the original pandigital number and its reduced value
   after stripping all factors of 2 and 5. */
typedef struct {
    uint64_t orig;      /* original 10‑digit pandigital */
    uint64_t reduced;   /* orig / 2^k / 5^m (k,m maximal) */
} node_t;

/* Recursively generate all 10‑digit pandigital numbers
   with no leading zero, storing each in the array. */
static void gen_perm(int pos, uint16_t used, uint64_t cur,
                     node_t *nodes, size_t *cnt)
{
    if (pos == 10) {
        uint64_t r = cur;
        /* strip powers of 2 */
        while ((r & 1ULL) == 0) r >>= 1;
        /* strip powers of 5 */
        while (r % 5ULL == 0) r /= 5;
        nodes[(*cnt)].orig     = cur;
        nodes[(*cnt)].reduced  = r;
        ++(*cnt);
        return;
    }
    for (int d = 0; d <= 9; ++d) {
        if (used & (1U << d)) continue;
        if (pos == 0 && d == 0) continue;        /* avoid leading 0 */
        gen_perm(pos + 1, used | (1U << d),
                 cur * 10ULL + (uint64_t)d,
                 nodes, cnt);
    }
}

/* Comparator for qsort: first by reduced value, then by original. */
static int cmp_node(const void *a, const void *b)
{
    const node_t *x = (const node_t *)a;
    const node_t *y = (const node_t *)b;
    if (x->reduced < y->reduced) return -1;
    if (x->reduced > y->reduced) return 1;
    if (x->orig < y->orig) return -1;
    if (x->orig > y->orig) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* 9 * 9! = 3 265 920 ten‑digit pandigital numbers without a leading zero. */
    const size_t CAP = 3265920U;
    static node_t *nodes = NULL;
    static bool built = false;

    if (!built) {
        nodes = (node_t *)malloc(CAP * sizeof(node_t));
        if (!nodes) return 0;          /* allocation failed */
        size_t cnt = 0;
        gen_perm(0, 0, 0ULL, nodes, &cnt);
        qsort(nodes, cnt, sizeof(node_t), cmp_node);
        built = true;
    }

    if (n == 0 || (size_t)n > CAP) return 0;
    return (seq_t)nodes[(size_t)(n - 1)].orig;
}
