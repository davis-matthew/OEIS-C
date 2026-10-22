/*
 * A365410  Parity of A000070.
 * https://oeis.org/A365410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000035(A000070(n))
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*                = a(n‑1) ⊕ P(n)   (mod 2)   with a(0)=1 */
/* P(n) = parity of the partition number p(n).  */
/* The parity of p(n) satisfies */
/*   P(0)=1,  P(n)=⊕_{g≤n} P(n−g)  where g runs over the generalized pentagonal
 *   numbers g = k(3k−1)/2, k(3k+1)/2, k=1,2,3,…  (signs ignored modulo 2). */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static cache of parity of p(k) and of the sequence itself. */
    static unsigned char *parity_p = NULL;   /* parity of p(k) 0/1 */
    static unsigned char *a_cache   = NULL;   /* parity of a(k) 0/1 */
    static idx_t max_n = 0;                   /* largest index cached */

    /* Initial allocation for index 0. */
    if (!parity_p) {
        parity_p = (unsigned char *)malloc(sizeof(unsigned char));
        if (!parity_p) return (seq_t)0;
        parity_p[0] = 1;  /* p(0)=1 → odd */

        a_cache = (unsigned char *)malloc(sizeof(unsigned char));
        if (!a_cache) {
            free(parity_p);
            parity_p = NULL;
            return (seq_t)0;
        }
        a_cache[0] = 1;    /* a(0) defined as 1 */
        max_n = 0;
    }

    /* Extend cache up to the requested index n. */
    if (n > max_n) {
        size_t new_size = (size_t)(n + 1);                /* cast to size_t for realloc */
        unsigned char *tmp_p = (unsigned char *)realloc(parity_p, new_size);
        unsigned char *tmp_a = (unsigned char *)realloc(a_cache, new_size);
        if (!tmp_p || !tmp_a) {
            /* If realloc failed we keep the old data; return existing value if possible. */
            if (n <= max_n) return (seq_t)a_cache[n];
            return (seq_t)0;
        }
        parity_p = tmp_p;
        a_cache   = tmp_a;

        /* Compute values for indices max_n+1 … n. */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            /* Compute parity of p(i). */
            unsigned char p_i = 0;  /* XOR accumulator */
            idx_t k = 1;
            for (;;) {
                /* Generalized pentagonal numbers g1 = k(3k−1)/2 and g2 = k(3k+1)/2*/
                idx_t g1 = k * (3 * k - 1) / 2;
                if (g1 > i) break;
                p_i ^= parity_p[i - g1];
                idx_t g2 = k * (3 * k + 1) / 2;
                if (g2 <= i) p_i ^= parity_p[i - g2];
                ++k;
            }
            parity_p[i] = p_i;

            /* a(i) = a(i−1) ⊕ p(i) (mod 2) */
            a_cache[i] = a_cache[i - 1] ^ p_i;
        }
        max_n = n;
    }

    return (seq_t)a_cache[n];
}
