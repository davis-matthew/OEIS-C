/*
 * A287774  Positions of 0 in A287773; complement of A287777.
 * https://oeis.org/A287774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th position of 0 in A287773, where A287773 is the
 *            Fibonacci word A003849 (fixed point of 0->01, 1->0) mapped by
 *            0->010, 1->101. Computed by generating the word and scanning it
 * Range:     u8 n <= 138, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 8877 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static unsigned char *A287774_w = NULL;
static size_t A287774_len = 0;

/* Make sure the stored Fibonacci-word prefix has at least `need` symbols. */
static int A287774_ensure(size_t need)
{
    if (A287774_len >= need) return 1;
    size_t cap = need * 2 + 8;
    unsigned char *w = (unsigned char *)malloc(cap);
    if (!w) return 0;
    /* Fixed point of 0->01, 1->0: start with sigma(0) = "01",
       then expand w[1], w[2], ... and append each image. */
    size_t j = 0, i = 1;
    w[j++] = 0;
    w[j++] = 1;
    while (j < need) {
        if (w[i] == 0) { w[j++] = 0; w[j++] = 1; }
        else           { w[j++] = 0; }
        i++;
    }
    free(A287774_w);
    A287774_w = w;
    A287774_len = j;
    return 1;
}

seq_t a(idx_t n)
{
    size_t need = (size_t)(n * 2 / 3 + 6);
    if (!A287774_ensure(need)) return 0;
    idx_t count = 0;
    idx_t base = 1;                 /* 1-based position of block k's first symbol */
    for (size_t k = 0; k < A287774_len; k++, base += 3) {
        if (A287774_w[k] == 0) {    /* block 010: zeros at offsets 0 and 2 */
            if (count + 1 == n) return (seq_t)base;
            if (count + 2 == n) return (seq_t)(base + 2);
            count += 2;
        } else {                    /* block 101: zero at offset 1 */
            if (count + 1 == n) return (seq_t)(base + 1);
            count += 1;
        }
    }
    return 0;
}
