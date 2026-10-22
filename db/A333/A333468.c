/*
 * A333468  Length of the largest disjoint cycle of the permutation that
 *          results from the composition of first n circular shifts.
 * https://oeis.org/A333468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the largest cycle in the permutation obtained
 *            by composing the first n circular shifts
 *            (1)*(1,2)*(1,2,3)*…*(1,2,…,n)
 * Range:     u8 n <= 260, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static idx_t *perm = NULL;           /* current permutation (1-indexed)   */
static idx_t *tmp  = NULL;           /* temporary buffer for shifting     */
static unsigned char *visited = NULL;/* visited flag array                 */
static seq_t *a_arr = NULL;          /* cached a(k) values                */
static idx_t max_prepared = 0;       /* largest n for which a(k) filled   */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    if (n > max_prepared) {
        idx_t old      = max_prepared;
        idx_t newMax   = n;

        /* Resize arrays to accommodate up to newMax */
        idx_t *new_perm  = (idx_t*)realloc(perm,
                                          (size_t)(newMax + 1) * sizeof(idx_t));
        idx_t *new_tmp   = (idx_t*)realloc(tmp,
                                          (size_t)(newMax + 1) * sizeof(idx_t));
        unsigned char *new_visited =
            (unsigned char*)realloc(visited,
                                    (size_t)(newMax + 1) * sizeof(unsigned char));
        seq_t *new_a_arr = (seq_t*)realloc(a_arr,
                                           (size_t)(newMax + 1) * sizeof(seq_t));
        if (!new_perm || !new_tmp || !new_visited || !new_a_arr)
            return (seq_t)0; /* allocation failure */

        perm      = new_perm;
        tmp       = new_tmp;
        visited   = new_visited;
        a_arr     = new_a_arr;

        /* Initialize newly added permutation entries as identity */
        for (idx_t i = old + 1; i <= newMax; ++i)
            perm[i] = i;

        /* Build permutation and a(k) for k = old+1 .. newMax */
        for (idx_t k = old + 1; k <= newMax; ++k) {
            /* copy current first k entries */
            for (idx_t i = 1; i <= k; ++i)
                tmp[i] = perm[i];

            /* apply the k‑cycle shift: i -> i+1 (mod k) */
            for (idx_t i = 1; i <= k; ++i) {
                idx_t src = (i == k) ? (idx_t)1 : i + 1;
                perm[i] = tmp[src];
            }

            /* compute longest cycle in perm[1..k] */
            memset(visited + 1, 0, (size_t)k);
            seq_t longest = (seq_t)1;
            for (idx_t i = 1; i <= k; ++i) {
                if (!visited[i]) {
                    idx_t cur = i;
                    seq_t len = (seq_t)0;
                    do {
                        visited[cur] = 1;
                        ++len;
                        cur = perm[cur];
                    } while (cur != i);
                    if (len > longest)
                        longest = len;
                }
            }
            a_arr[k] = longest;
        }
        max_prepared = newMax;
    }
    return a_arr[n];
}
