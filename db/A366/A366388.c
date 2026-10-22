/*
 * A366388  The number of edges minus the number of leaves in the rooted tree
 *          with Matula-Goebel number n.
 * https://oeis.org/A366388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is totally additive with a(2)=0 and for n>1, a(prime(n)) =
 *            1 + a(n). For even n, a(2m)=a(m); for odd composites, a(n) =
 *            a(p) + a(n/p) where p is the smallest prime factor of n
 * Range:     u8 n <= 9581, u16 n <= 9581, u32 n <= 9581, u64 n <= 9581,
 *            u128 n <= 9581, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n<1, but return 0 safely */

    /* Allocate arrays */
    idx_t *spf = (idx_t*)malloc((n + 1) * sizeof(idx_t));
    if (!spf) return 0; /* allocation failure */

    /* smallest prime factor sieve */
    for (idx_t i = 0; i <= n; i++) spf[i] = i;
    for (idx_t i = 2; i * i <= n; i++) {
        if (spf[i] == i) {
            for (idx_t j = i * i; j <= n; j += i) {
                if (spf[j] == j) spf[j] = i;
            }
        }
    }

    /* Mapping from prime value to its index in the prime sequence */
    unsigned int *prime_idx = (unsigned int*)calloc(n + 1, sizeof(unsigned int));
    if (!prime_idx) { free(spf); return 0; }
    unsigned int count = 0;
    for (idx_t i = 2; i <= n; i++) {
        if (spf[i] == i) { /* i is prime */
            count++;
            prime_idx[i] = count; /* 1‑based index */
        }
    }

    /* Compute a(n) for all 1..n */
    seq_t *a_arr = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!a_arr) { free(spf); free(prime_idx); return 0; }

    a_arr[1] = 0;
    if (n >= 2) a_arr[2] = 0;

    for (idx_t i = 3; i <= n; i++) {
        if ((i & 1) == 0) {           /* even */
            a_arr[i] = a_arr[i >> 1];
        } else if (spf[i] == i) {     /* prime */
            if (i == 2)               /* already handled, but keep for safety */
                a_arr[i] = 0;
            else
                a_arr[i] = a_arr[prime_idx[i]] + 1;
        } else {                      /* odd composite */
            idx_t p = spf[i];
            idx_t rest = i / p;
            a_arr[i] = a_arr[p] + a_arr[rest];
        }
    }

    seq_t result = a_arr[n];

    /* Clean up */
    free(spf);
    free(prime_idx);
    free(a_arr);

    return result;
}
