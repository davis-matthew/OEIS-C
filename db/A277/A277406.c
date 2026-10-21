/*
 * A277406  a(n) equals the sum of all permutations of compositions of
 *          functions (1 + k*x) for k=1..n, evaluated at x=1.
 * https://oeis.org/A277406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{k=1..n} k! * (n-k)! * e_k(n) where e_k(n) =
 *            Σ_{|S|=k} Π_{i∈S} i is the k-th elementary symmetric sum of the
 *            integers 1..n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(0) = 1  */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Cache structure: on first call initialize for n==0.
       Subsequent calls may extend the cache up to the largest n requested. */
    static idx_t max_n = 0;          /* highest n cached so far               */
    static seq_t *fact = NULL;       /* factorials fact[0..max_n]             */
    static seq_t *e    = NULL;       /* elementary sums e[0..max_n]           */
    static seq_t *aval = NULL;       /* cached a(n) values                    */

    if (n == 0)            /* a(0) = 1 by convention */
        return (seq_t)1;

    /* Extend the cache if necessary */
    if (n > max_n) {
        idx_t new_max = n;

        /* Resize factorial array */
        fact = (seq_t *)realloc(fact, (new_max + 1) * sizeof(seq_t));
        /* Resize elementary sums array */
        e = (seq_t *)realloc(e, (new_max + 1) * sizeof(seq_t));
        /* Resize a(n) cache array */
        aval = (seq_t *)realloc(aval, (new_max + 1) * sizeof(seq_t));

        /* First call initialisation */
        if (max_n == 0) {
            fact[0] = (seq_t)1;
            e[0]    = (seq_t)1;   /* e_0 = 1 by definition */
            aval[0] = (seq_t)1;
        }

        /* Compute values for i = max_n+1 .. new_max */
        for (idx_t i = max_n + 1; i <= new_max; ++i) {
            /* factorial */
            fact[i] = fact[i - 1] * (seq_t)i;

            /* elementary symmetric sums by the recurrence
               e[t] = e[t] + i * e[t-1]   for t = i..1
               Ensure e[i] starts from zero for each new i. */
            e[i] = (seq_t)0;
            for (idx_t t = i; t > 0; --t) {
                e[t] += (seq_t)i * e[t - 1];
            }

            /* compute a(i) = 1 + Σ_{k=1..i} k!*(i-k)!*e[k] */
            seq_t sum = fact[i];          /* start with the k = i term (k! = (i)! , (i-k)! = 0! = 1) */
            for (idx_t k = 1; k <= i; ++k) {
                seq_t prod = fact[i - k] * fact[k];
                prod *= e[k];
                sum += prod;
            }
            aval[i] = sum;
        }

        max_n = new_max;
    }

    return aval[n];
}
