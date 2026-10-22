/*
 * A325426  Complement of A325425.
 * https://oeis.org/A325426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 117, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A325426: Complement of A325425
 * 
 *  A325426 consists of all integers that can be expressed as
 *      2*x   or   floor(3*x/2)
 *  for some integer x > 1 that belongs to A325425.
 * 
 *  A325425 is the greedy set of integers that cannot be represented
 *  by these transformations from earlier members of the set.
 *  All other integers are in A325426, the complement of A325425.
 * 
 *  This implementation builds A325425 lazily as it scans the
 *  natural numbers in increasing order, marking numbers that belong
 *  to A325426 in a dynamic boolean array.  When the n‑th marked
 *  number is found it is returned as the answer.
 *
 *  Complexity:  O(n) time and O(n) memory for n up to about 10 000
 *  (faster for typical inputs).  All indices are uint64_t (idx_t);
 *  the result is returned as seq_t (suitably typed for big‑int builds).
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0)          /* offset starts at 1; safeguard */
        return 0;

    idx_t count = 0;                 /* number of elements of A325426 seen */
    size_t cap = 128;                /* current allocation in elements */
    unsigned char *visited = (unsigned char*)calloc(cap, sizeof(unsigned char));
    if (!visited) return 0;          /* allocation failure */

    for (idx_t i = 1;; ++i)
    {
        /* Ensure the array covers the current index i */
        if ((size_t)i >= cap)
        {
            size_t new_cap = cap * 2;
            if (new_cap < (size_t)(i + 1))
                new_cap = i + 1;
            unsigned char *tmp = (unsigned char*)realloc(visited,
                                                        new_cap * sizeof(unsigned char));
            if (!tmp)
            {
                free(visited);
                return 0;
            }
            memset(tmp + cap, 0, (new_cap - cap) * sizeof(unsigned char));
            visited = tmp;
            cap = new_cap;
        }

        if (visited[i])            /* i ∈ A325426 (already covered) */
        {
            ++count;
            if (count == n)
            {
                free(visited);
                return (seq_t)i;
            }
        }
        else                        /* i ∈ A325425, generate its successors */
        {
            /* mark 2*i */
            idx_t a1 = i + i;
            if (a1 >= cap)
            {
                size_t new_cap = cap;
                if (a1 >= new_cap) new_cap = a1 + 1;
                if (new_cap < cap * 2) new_cap = cap * 2;
                unsigned char *tmp = (unsigned char*)realloc(visited,
                                                            new_cap * sizeof(unsigned char));
                if (!tmp)
                {
                    free(visited);
                    return 0;
                }
                memset(tmp + cap, 0, (new_cap - cap) * sizeof(unsigned char));
                visited = tmp;
                cap = new_cap;
            }
            visited[a1] = 1;

            /* mark floor(3*i/2) if i>1 */
            if (i > 1)
            {
                idx_t a2 = (i * 3) / 2;
                if (a2 >= cap)
                {
                    size_t new_cap = cap;
                    if (a2 >= new_cap) new_cap = a2 + 1;
                    if (new_cap < cap * 2) new_cap = cap * 2;
                    unsigned char *tmp = (unsigned char*)realloc(visited,
                                                                new_cap * sizeof(unsigned char));
                    if (!tmp)
                    {
                        free(visited);
                        return 0;
                    }
                    memset(tmp + cap, 0, (new_cap - cap) * sizeof(unsigned char));
                    visited = tmp;
                    cap = new_cap;
                }
                visited[a2] = 1;
            }
        }
    }
}
