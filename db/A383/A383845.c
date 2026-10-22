/*
 * A383845  Triangle T(n,k) read by rows: where T(n,k) is the number of the
 *          k-th eliminated person in the variation of the Josephus
 *          elimination process for n people, where the elimination pattern is
 *          eliminate-eliminate-skip.
 * https://oeis.org/A383845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence: A383845 – Josephus “eliminate–eliminate–skip” triangle.
   a(n) is the k‑th element in the elimination order of the row r,  
   where r is the smallest integer with r*(r+1)/2 ≥ n and  
   k = n − (r−1)*r/2.  The removal is performed by simulating the
   process: eliminate two consecutive persons and skip one, repeating
   until the circle is empty.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find the row r such that T(r) = r*(r+1)/2 ≥ n */
    idx_t r = 1;
    while ((seq_t)r * (r + 1) / 2 < (seq_t)n)
        ++r;

    /* Position of n within that row (1‑based) */
    idx_t prev_tri = (r - 1) * r / 2;   /* T(r-1) */
    idx_t k = n - prev_tri;             /* elimination number needed */

    /* Allocate and initialize the circle of r people (1‑based indices) */
    idx_t *people = (idx_t *)malloc(r * sizeof(idx_t));
    if (!people) return (seq_t)0;       /* allocation failure – unlikely */
    for (idx_t i = 0; i < r; ++i)
        people[i] = i + 1;

    idx_t size = r;        /* current number of people in the circle */
    idx_t cur   = 0;       /* current index in the circle */
    idx_t step  = 0;       /* step counter (eliminate/eliminate/skip cycle) */
    idx_t elim_count = 0;  /* how many people have been eliminated */

    while (elim_count < k) {
        if (step % 3 == 2) {                 /* skip step */
            cur = (cur + 1) % size;
        } else {                             /* elimination step */
            seq_t val = (seq_t)people[cur];
            ++elim_count;
            if (elim_count == k) {
                free(people);
                return val;
            }

            /* Remove the eliminated person by shifting left */
            for (idx_t i = cur; i + 1 < size; ++i)
                people[i] = people[i + 1];
            --size;

            if (cur == size) cur = 0;        /* wrap around if needed */
        }
        ++step;
    }

    /* The loop should always return before reaching here */
    free(people);
    return (seq_t)0;
}
