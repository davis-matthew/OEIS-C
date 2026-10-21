/*
 * A262669  Consider the Farey sequence of order n, F_n, and that the average
 *          distance between any two adjacent pairs in F_n is 1/A002088(n).
 *          Then a(n) is the number of adjacent pairs whose difference is less
 *          than the average.
 * https://oeis.org/A262669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of adjacent Farey pairs in F_n whose difference
 *            1/(b·d) is less than the average 1/(A002088(n)-1). An adjacent
 *            pair a/b, c/d satisfies b·d < (A002088(n)-1), where
 *            A002088(n)=1+∑φ(k) for 1≤k≤n
 * Range:     u8 n <= 50, u16 n <= 805, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* external types supplied by the build */
seq_t a(idx_t n)
{
    /* For n=0 or 1 the Farey sequence has fewer than 2 fractions
       so there are no adjacent pairs. */
    if (n <= 1) return 0;

    /* Allocate and fill φ(1..n).  φ(0) is unused. */
    idx_t *phi = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    if (!phi) return 0;  /* allocation failure → return 0 */

    for (idx_t i = 0; i <= n; ++i) phi[i] = i;

    /* Sieve to compute φ. */
    for (idx_t p = 2; p <= n; ++p) {
        if (phi[p] == p) {          /* p is prime */
            for (idx_t j = p; j <= n; j += p) {
                phi[j] -= phi[j] / p;
            }
        }
    }

    /* Sum of φ(k) gives the number of adjacent pairs.
       (Total fractions = sumφ + 1; pairs = total - 1 = sumφ.) */
    uint64_t sum_phi = 0;
    for (idx_t i = 1; i <= n; ++i) sum_phi += (uint64_t)phi[i];
    uint64_t intervals = sum_phi;          /* equals A002088(n)-1 */

    free(phi);

    /* Generate the Farey sequence via the mediant algorithm
       and count pairs with product of denominators < intervals. */
    idx_t a_num = 0, a_den = 1;   /* current fraction a/b */
    idx_t c_num = 1, c_den = n;   /* next fraction c/d */
    seq_t count = 0;              /* result to return */

    for (;;) {
        /* Count if product of denominators is less than intervals. */
        uint64_t prod = (uint64_t)a_den * (uint64_t)c_den;
        if (prod < intervals) ++count;

        /* Stop after the last fraction 1/1 has been processed. */
        if (c_num == 1 && c_den == 1) break;

        /* Compute the next fraction in the Farey sequence. */
        idx_t k   = (n + a_den) / c_den;
        idx_t e_num = k * c_num - a_num;
        idx_t e_den = k * c_den - a_den;

        a_num = c_num;
        a_den = c_den;
        c_num = e_num;
        c_den = e_den;
    }

    return count;
}
