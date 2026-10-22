/*
 * A340734  Number of partitions of 2n into 4 parts that divide 2n.
 * https://oeis.org/A340734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of partitions of 2n into 4 positive integers that
 *            divide 2n
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Comparator for qsort on seq_t values */
static int cmp_seq(const void *p1, const void *p2) {
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

/* Count partitions of 2n into 4 positive parts all dividing 2n */
seq_t a(idx_t n) {
    seq_t two_n = (seq_t)n + (seq_t)n;          /* 2 * n */
    if (two_n < 4) return 0;                   /* minimal sum for 4 parts is 4 */

    /* Build a lookup for divisors of two_n */
    size_t look_size = (size_t)two_n + 1;
    unsigned char *div_lookup = (unsigned char *)calloc(look_size, sizeof(unsigned char));
    if (!div_lookup) return 0;                  /* allocation failure */

    /* Collect divisors (unsorted) */
    size_t est = 64;
    seq_t *divs   = (seq_t *)malloc(est * sizeof(seq_t));
    size_t dcnt   = 0;

    for (size_t i = 1; (seq_t)i * (seq_t)i <= two_n; ++i) {
        if (two_n % i == 0) {
            if (dcnt >= est) { est <<= 1; divs = (seq_t *)realloc(divs, est * sizeof(seq_t)); }
            divs[dcnt++] = (seq_t)i;
            div_lookup[i] = 1;

            size_t j = (size_t)(two_n / i);
            if (j != i) {
                if (dcnt >= est) { est <<= 1; divs = (seq_t *)realloc(divs, est * sizeof(seq_t)); }
                divs[dcnt++] = (seq_t)j;
                div_lookup[j] = 1;
            }
        }
    }

    /* Sort divisors */
    qsort(divs, dcnt, sizeof(seq_t), cmp_seq);

    seq_t count = 0;

    /* Enumerate sorted quadruples (a,b,c,d) with a<=b<=c<=d */
    for (size_t ia = 0; ia < dcnt; ++ia) {
        seq_t a_part = divs[ia];
        for (size_t ib = ia; ib < dcnt; ++ib) {
            seq_t b_part = divs[ib];
            for (size_t ic = ib; ic < dcnt; ++ic) {
                seq_t c_part = divs[ic];
                seq_t sum3 = a_part + b_part + c_part;
                if (sum3 > two_n) break;                 /* further c's will only increase sum3 */
                seq_t d_part = two_n - sum3;
                if (d_part < c_part) continue;           /* maintain sorted order */
                if (d_part <= two_n && div_lookup[(size_t)d_part])
                    ++count;
            }
        }
    }

    free(divs);
    free(div_lookup);
    return count;
}
