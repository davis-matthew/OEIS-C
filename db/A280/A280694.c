/*
 * A280694  Largest Lucas number (A000032) dividing n.
 * https://oeis.org/A280694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 320, u16 n <= 8955, u32 n <= 8955, u64 n <= 8955,
 *            u128 n <= 8955, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = largest Lucas number that divides n */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1 – defensive */

    /* convert index to seq_t for arithmetic and modulus */
    seq_t sn = (seq_t)n;

    /* static cache for Lucas numbers */
    static seq_t *lucas   = NULL;
    static size_t lucas_cap = 0;
    static size_t lucas_len = 0;

    /* initialise cache on first call */
    if (lucas_len == 0) {
        lucas_cap = 8;                          /* arbitrary start size */
        lucas = (seq_t *)malloc(lucas_cap * sizeof(seq_t));
        if (lucas == NULL) return 0;            /* allocation failure unlikely */
        lucas[0] = 2;  /* L0 */
        lucas[1] = 1;  /* L1 */
        lucas_len = 2;
    }

    /* generate Lucas numbers ≤ sn, extending the cache as needed */
    while (lucas[lucas_len - 1] <= sn) {
        /* make space for the next Lucas number */
        if (lucas_len == lucas_cap) {
            size_t new_cap = lucas_cap * 2;
            seq_t *tmp = (seq_t *)realloc(lucas, new_cap * sizeof(seq_t));
            if (tmp == NULL) return 0;           /* allocation failure */
            lucas = tmp;
            lucas_cap = new_cap;
        }

        seq_t aprev = lucas[lucas_len - 2];
        seq_t bprev = lucas[lucas_len - 1];

        /* prevent overflow and ensure next does not exceed sn */
        if (sn < aprev) break;
        if (sn - aprev < bprev) break;

        seq_t next = aprev + bprev;
        if (next > sn) break;                   /* no need to store > sn */

        lucas[lucas_len++] = next;
    }

    /* find the largest Lucas number that divides sn */
    seq_t largest = 1;                          /* L1 divides every n ≥ 1 */
    for (size_t i = 0; i < lucas_len; ++i) {
        if (sn % lucas[i] == 0) {
            if (lucas[i] > largest) largest = lucas[i];
        }
    }

    return largest;
}
