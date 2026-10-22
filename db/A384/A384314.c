/*
 * A384314  Numbers k such that the nonzero digits in the ternary expansion k
 *          = d(1),...,d(m) satisfy d(2*i+1) = d(1) and d(2*i) = 3-d(1).
 * https://oeis.org/A384314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A384314: numbers whose non‑zero ternary digits alternate.
   Enumerate all such numbers up to the required length, sort them,
   and return the n‑th value (1‑based). */

#include <stdlib.h>

static int cmp_seq(const void *a, const void *b)
{
    const seq_t *va = (const seq_t *)a;
    const seq_t *vb = (const seq_t *)b;
    return (*va < *vb) ? -1 : (*va > *vb) ? 1 : 0;
}

seq_t a(idx_t n)
{
    /* undefined for n == 0, return 0 as a safe value */
    if (n == 0) return (seq_t)0;

    /* the first term is 0 */
    if (n == 1) return (seq_t)0;

    /* find the smallest ternary length Lmax such that
       2^(Lmax+1) - 1 >= n  (total terms up to that length)     */
    size_t Lmax = 0;
    idx_t pow = 2;  /* 2^(L+1) when L = 0 is 2^(1) */
    while ((idx_t)(pow - 1) < n) {
        Lmax++;
        pow <<= 1;          /* multiply by 2 each iteration   */
    }
    /* capacity in elements : 2^(Lmax+1)-1 */
    size_t capacity = (size_t)(pow - 1);

    seq_t *arr = (seq_t *)malloc(capacity * sizeof(seq_t));
    if (!arr) abort();
    size_t pos = 0;
    arr[pos++] = (seq_t)0;          /* a(1) = 0 */

    /* pre‑compute powers of 3 up to Lmax */
    seq_t *pow3 = (seq_t *)malloc((Lmax + 1) * sizeof(seq_t));
    if (!pow3) { free(arr); abort(); }
    pow3[0] = 1;
    for (size_t i = 1; i <= Lmax; ++i)
        pow3[i] = pow3[i - 1] * (seq_t)3;

    /* generate all numbers of lengths 1..Lmax */
    for (size_t L = 1; L <= Lmax; ++L) {
        for (int d1 = 1; d1 <= 2; ++d1) {
            if (L == 1) {
                arr[pos++] = (seq_t)d1;
            } else {
                size_t masks = ((size_t)1 << (L - 1));
                for (size_t mask = 0; mask < masks; ++mask) {
                    seq_t val = (seq_t)d1 * pow3[L - 1];
                    for (size_t j = 1; j < L; ++j) { /* j is zero‑based offset from the most significant digit */
                        if (mask & ((size_t)1 << (j - 1))) {
                            /* position p = j+1 (1‑based):
                               if p is even → digit = 3‑d1,
                               if p is odd  → digit = d1.          */
                            int digit = ((j & 1) == 1) ? (3 - d1) : d1;
                            val += (seq_t)digit * pow3[L - 1 - j];
                        }
                    }
                    arr[pos++] = val;
                }
            }
        }
    }

    free(pow3);

    /* arr now contains exactly 'capacity' elements */
    qsort(arr, capacity, sizeof(seq_t), cmp_seq);

    seq_t result = arr[n - 1];
    free(arr);
    return result;
}
