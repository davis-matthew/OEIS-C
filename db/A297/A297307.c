/*
 * A297307  Given n, define the sequence x(1) = n, thereafter if x(i) is even
 *          set x(i+1) = x(i)/2, if x(i) is odd and divisible by 3, 5 or 7 set
 *          x(i+1) = 5*x(i) + 1, otherwise set x(i+1) = 3*x(i) + 1. Then a(n)
 *          is the smallest i>1 such that x(i) = 1, 5, or 553, or -1 if none
 *          of those numbers is ever reached.
 * https://oeis.org/A297307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 30, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS sequence A297307:
   a(n) = smallest i>1 such that the sequence x defined by
   x(1)=n, x(i+1) = { x(i)/2 if x(i) even,
                      5*x(i)+1 if x(i) odd and divisible by 3,5,7,
                      3*x(i)+1 otherwise }
   equals 1, 5, or 553.  Returns -1 if none of those values is ever
   reached.  The function works for any seq_t and idx_t types. */
/* NOTE: Uses a visited‑value table to detect cycles.  In C++ the table
   is a std::vector; in C it is a dynamically resized array. */
#include <stdbool.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

static inline seq_t next_value(seq_t x)
{
    if ((x & 1) == 0)          /* even */
        return x >> 1;
    else if ((x % 3) == 0 || (x % 5) == 0 || (x % 7) == 0)
        return x * 5 + 1;       /* odd and divisible by 3,5,7 */
    else
        return x * 3 + 1;       /* odd and not divisible by 3,5,7 */
}

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;            /* current value of the sequence   */
    seq_t target1 = (seq_t)1;
    seq_t target2 = (seq_t)5;
    seq_t target3 = (seq_t)553;
    idx_t steps = 1;               /* i starts at 1 for x(1)=n */

    /* visited value table (for cycle detection) */
#ifdef __cplusplus
    std::vector<seq_t> visited;
    visited.reserve(16);
#else
    seq_t *visited = NULL;
    size_t visited_cap = 16;
    size_t visited_cnt = 0;
    visited = (seq_t *)malloc(visited_cap * sizeof(seq_t));
    if (!visited) return (seq_t)-1; /* allocation failure */
#endif

    /* add starting value to the table */
#ifdef __cplusplus
    visited.push_back(x);
#else
    visited[visited_cnt++] = x;
#endif

    for (;;)
    {
        x = next_value(x);
        ++steps;

        /* Check for target values */
        if (x == target1 || x == target2 || x == target3) {
#ifdef __cplusplus
            return steps;
#else
            free(visited);
            return steps;
#endif
        }

        /* Check if x has been seen before (cycle detection) */
        bool found = false;
#ifdef __cplusplus
        for (size_t i = 0; i < visited.size(); ++i)
            if (visited[i] == x) { found = true; break; }
#else
        for (size_t i = 0; i < visited_cnt; ++i)
            if (visited[i] == x) { found = true; break; }
#endif
        if (found) {
#ifdef __cplusplus
            return (seq_t)-1;
#else
            free(visited);
            return (seq_t)-1;
#endif
        }

        /* Add x to visited list, grow if necessary */
#ifdef __cplusplus
        visited.push_back(x);
#else
        if (visited_cnt >= visited_cap) {
            visited_cap <<= 1;
            seq_t *newp = (seq_t *)realloc(visited, visited_cap * sizeof(seq_t));
            if (!newp) {
                free(visited);
                return (seq_t)-1; /* allocation failure */
            }
            visited = newp;
        }
        visited[visited_cnt++] = x;
#endif
    }
}
