/*
 * A257242  Random Fibonacci tree defined with the pair(1,1).
 * https://oeis.org/A257242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(1)=1, a(2)=1; for k≥2, a(2k-1)=abs(a[(k+1)/2]-a[k]),
 *            a(2k)=a[(k+1)/2]+a[k]
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Breadth‑first labels of the random Fibonacci tree with pair (1,1). */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* offset starts at 1; unused below */

    if (n <= 2) return 1;                      /* a(1)=1, a(2)=1 */

    /* allocate space for indices 1..n */
    size_t sz = (size_t)(n + 1);
    seq_t *val = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!val) return 0;                         /* allocation failure */

    val[1] = 1;                                  /* root */
    val[2] = 1;                                  /* first child of the root */

    for (idx_t k = 2; (2 * k - 1) <= n; ++k) {
        idx_t parent = (k + 1) / 2;              /* parent index of node k */
        seq_t parent_val = val[parent];
        seq_t node_val    = val[k];

        /* children of node k */
        seq_t left_child  = (parent_val >= node_val)
                            ? (parent_val - node_val)
                            : (node_val - parent_val);   /* abs(...) */
        seq_t right_child = parent_val + node_val;

        idx_t left_idx  = 2 * k - 1;
        idx_t right_idx = 2 * k;

        if (left_idx <= n)  val[left_idx]  = left_child;  /* |x-y|  */
        if (right_idx <= n) val[right_idx] = right_child; /* x+y    */
    }

    seq_t result = val[n];
    free(val);
    return result;
}
